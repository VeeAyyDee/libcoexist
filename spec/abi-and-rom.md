# ABI, runtime storage and ROM integration

Target ABI is 32-bit little-endian Xtensa ESP32-S3, with 32-bit data/function pointers. Firmware structures must satisfy exact size/offset assertions. Host tests must model target addresses independently; native 64-bit pointer structures will not satisfy target offsets. ELF section/alignment facts are in the inventories. A section alignment is not necessarily a source type alignment requirement. Original .coexiram functions and .dram data imply placement constraints: runtime callbacks, literals, state and reachable helpers need the appropriate internal-memory placement when invoked with flash cache unavailable.

## Adapter contract

The public S3 `coex_adapter_funcs_t` is 80 bytes. Version is at byte 0 and magic at byte 76. Version 2 and magic 0xDEADBEAF are public SDK interface constants. These S3 offsets exclude ESP32-only spinlock members and ESP32C2-only clock calibration members.

| Offset | Public SDK member |
|---|---|
| 4 | task_yield_from_isr |
| 8 | semphr_create(maximum, initial) |
| 12 | semphr_delete |
| 16 | semphr_take_from_isr(handle, higher_priority_task_woken) |
| 20 | semphr_give_from_isr(handle, higher_priority_task_woken) |
| 24 | semphr_take(handle, timeout_ticks) |
| 28 | semphr_give |
| 32 | is_in_isr |
| 36 | malloc_internal |
| 40 | free |
| 44 | esp_timer_get_time (64-bit result) |
| 48 | env_is_chip |
| 52 | timer_disarm |
| 56 | timer_done |
| 60 | timer_setfn(timer, callback, argument) |
| 64 | timer_arm_us(timer, delay_us, repeat) |
| 68 | debug_matrix_init |
| 72 | xtal_freq_get |

Evidence: public `esp-idf/components/esp_coex/include/private/esp_coexist_adapter.h`; F:esp_coex_adapter_register, F:coex_core_lock, F:coex_rom_osi_funcs_init. These are offset facts, not copied adapter implementation.

## Published state layouts

`coex_env` is 16 bytes: semaphore pointer at 0, 32-bit core status at 4, an unresolved/reserved word at 8, start callback pointer at 12. The reference counter is separate private byte storage, not part of this exported object. Do not repurpose offset 8 until all ROM consumers have been examined. Evidence F:coex_core_enable, F:coex_core_disable, F:coex_core_register_start_cb; D:coex_env.

`coex_schm_env` is 64 bytes with these established accesses:

| Offset | Width | Meaning established by observed consumer |
|---|---:|---|
| 0 | 4 | current descriptor pointer |
| 4 | 1 | current phase index |
| 5 | 1 | unresolved/padding |
| 6 | 2 | Wi-Fi status bits |
| 8 | 2 | BLE status bits |
| 10 | 2 | classic BT status bits |
| 12 | 4 | schedule interval multiplier |
| 16 | 4 | scheduler semaphore |
| 20 | 4 | BT scheme callback |
| 24, 25 | 1 each | Wi-Fi primary and secondary channels |
| 26–27 | 2 | unresolved/padding |
| 28 | 4 | channel-change callback |
| 32–51 | 20 | adapter timer object storage; use pinned SDK timer ABI |
| 52 | 4 | Wi-Fi scheme callback |
| 56 | 2 | external-coexistence status bits |
| 58 | 1 | flexible period |
| 59 | 1 | unresolved/padding |
| 60 | 2 | I154 status bits |
| 62–63 | 2 | unresolved/padding |

These offsets are ROM-visible ABI, even when a function is a trivial accessor. A 32-bit read at offset 8 treats BLE and BT activity jointly for loop eligibility. Evidence F:coex_schm_is_loop_allowed and each named scheduler accessor. The timer reservation follows offsets 32 and 52, but the complete timer structure's semantics must come from SDK declarations, not assumed padding.

`coex_pti_tab` is 42 one-byte priority entries. `g_coex_param` is five 32-bit duration values, associated in order with events 4, 7, 9, 39 and 40. Policy values are intentionally withheld. `coex_log_level` is a writable 32-bit debug level. Three `g_coex_*md5` exports and two `libcoexist_reversion_*` exports are 4-byte string pointers, **not arrays of digest bytes**. Preserve pointer indirection. Build revision metadata may identify the fresh implementation honestly; interface identity strings must continue to identify the pinned SDK interface. Evidence D objects and F checksum/duration functions.

## ROM-owned pointer cells and dependencies

The SDK ROM linker map gives writable pointer cells, not storage owned by libcoexist:

| Symbol | S3 address | Role |
|---|---|---|
| coex_env_ptr | 0x3fcef834 | pointer to core state |
| coex_pti_tab_ptr | 0x3fcef830 | pointer to priority table |
| coex_schm_env_ptr | 0x3fcef82c | pointer to scheduler state |
| coexist_funcs | 0x3fcef828 | pointer to 48-byte dispatch table |
| g_coa_funcs_p | 0x3fcef824 | pointer to caller-owned adapter table |
| g_coex_param_ptr | 0x3fcef820 | pointer to duration parameters |

ROM functions referenced externally include `esp_coex_rom_version_get` (0x40005b68) and `coex_core_ble_conn_dyn_prio_get` (0x40005b8c). Remaining undefined symbols outside the archive are `coexist_printf`, `puts`, `strlen`, `strncmp`; their actual providers must be checked in the final map. Full dependency inventory includes the six cells above. This is not the whole call graph: adapter callbacks and memory-mapped registers are additional dependencies.

The SDK ROM map also defines names that the archive exports, including `coex_core_request`, `coex_core_release`, `coex_hw_timer_set` and scheduler locks. Merely linking an archive does not prove that those bodies were selected: archive extraction and absolute ROM assignments can satisfy symbols unexpectedly. Final firmware maps must show replacement ownership of **all intended functions and callback addresses**. The ROM `coex_core_ble_conn_dyn_prio_get` remains a dependency unless separately replaced under a verified contract. Therefore an all-original source archive is not a ROM-free radio stack.

## ROM publication and lifetime

`coex_rom_data_init` uses a private one-time byte guard. On the first call it publishes, in order, the core-state pointer, scheduler-state pointer, PTI-table pointer and parameter pointer, then marks initialization complete. Subsequent calls do nothing. Deinitializing the dispatch table does not reset this data-publication guard. Evidence F:coex_rom_data_init.

`coex_rom_osi_funcs_init` queries/logs the ROM version, calls data publication, and inspects `coexist_funcs`. A non-null existing dispatch pointer gives success without allocation or overwriting its contents. Otherwise it allocates exactly 48 internal bytes through the registered adapter. Null allocation returns 0x101. The original publishes the returned pointer **before** initializing the twelve slots below; this is an observed ordering, not evidence that concurrent readers are safe. Caller serialization/ROM-reader quiescence is a release prerequisite. Do not casually 'improve' ordering without deciding how strict compatibility and concurrency safety will be validated.

| Slot | Installed entry |
|---:|---|
| 0 | coex_core_pti_get |
| 1 | coex_core_timer_idx_get |
| 2 | coex_core_request |
| 3 | coex_core_release |
| 4 | coex_core_ble_conn_dyn_prio_get (ROM dependency) |
| 5 | coex_core_event_duration_get |
| 6 | coex_core_status_get |
| 7 | coex_hw_timer_set |
| 8 | coex_hw_timer_enable |
| 9 | coex_hw_timer_disable |
| 10 | coex_schm_lock |
| 11 | coex_schm_unlock |

`coex_rom_osi_funcs_deinit` frees the non-null dispatch allocation through adapter free, then clears the global pointer. It does not clear the four published data pointers or unregister the adapter. It must not race a ROM table consumer. Evidence F:coex_rom_osi_funcs_init/deinit. The fresh implementation needs a typed dispatch layout checked against this slot ordering; the table is a functional ABI, not vendor policy material.
