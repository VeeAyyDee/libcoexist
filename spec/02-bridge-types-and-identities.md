# Closure 02: ROM dispatch types, callback signature and identity tokens

This addendum supplies immediate bridge requirements for the fresh implementation. Public declarations plus observed caller/callee ABI establish these normalized signatures. The spelling of an unavailable vendor typedef is not recoverable from binary and is not required for ABI compatibility.

## Twelve ROM dispatch entries, in exact order

| Slot | Function | Compatible function type |
|---:|---|---|
| 0 | coex_core_pti_get | int (uint32_t event, uint8_t *priority) |
| 1 | coex_core_timer_idx_get | uint8_t (uint32_t event) |
| 2 | coex_core_request | int (uint8_t radio, uint32_t event, uint32_t latency, uint32_t duration) |
| 3 | coex_core_release | int (uint8_t radio, uint32_t event) |
| 4 | coex_core_ble_conn_dyn_prio_get | int (bool *low, bool *high) |
| 5 | coex_core_event_duration_get | int (uint32_t event, uint32_t *duration) |
| 6 | coex_core_status_get | uint32_t (void) |
| 7 | coex_hw_timer_set | void (uint8_t index, uint8_t radio, uint8_t priority, uint32_t latency, uint32_t duration) |
| 8 | coex_hw_timer_enable | void (uint8_t index) |
| 9 | coex_hw_timer_disable | void (uint8_t index) |
| 10 | coex_schm_lock | int (void) |
| 11 | coex_schm_unlock | int (void) |

All slots are 32-bit function pointers on S3, total 48 bytes. Core release preserves the unused radio argument position; deleting it would move event into the wrong register. Index result is always 0–3 or 255 in the pinned implementation. Timer source and priority explicitly narrow to bytes; duration narrows further to 24 hardware bits during programming. Timer return register remnants are not function results. The BLE dynamic-priority function remains a ROM dependency, with the bool-pointer signature corroborated by the public controller OS interface in `esp-idf/components/bt/controller/esp32c3/bt.c:225`. That source services the controller family but this declaration evidence alone does not prove the ROM function's whole behavior.

Hardware timer declarations are additionally corroborated by public controller source at `esp-idf/components/bt/controller/esp32c6/bt.c:137` through 139 and `esp32h2/bt.c:130` through 132. Original S3 byte narrowing and argument order are independently established by F:coex_hw_timer_set/enable/disable and the request caller. Core prototypes use F:coex_core_* plus F:coex_wifi_request/release and F:coex_bt_request/release. Exact installation ordering is in the original ABI document.

## Scheduler callback type

The scheduler callback is **int (*)(int event)**, not the older two-argument `coex_func_cb_t`. Public S3 Wi-Fi adapter source `esp-idf/components/esp_wifi/esp32s3/esp_adapter.c:582` declares the registration wrapper as accepting `int (*cb)(int)`. Public `esp-idf/components/esp_wifi/include/esp_private/wifi_os_adapter.h:150` confirms that callback type. The scheduler passes event 0 and ignores the int result. The BT callback observed in the original controller likewise consumes one int and returns 0 (supervisor-private object `research/btdm_initial/extracted/libbtdm_app/0019_coex_schm.o`, function `coex_schm_btdm_callback`). Store/call this function through a compatible typed function pointer in the fresh source; a void-return prototype or two-argument callback typedef is not the established interface.

## Required compatibility tokens

| Exported pointer | NUL-terminated token |
|---|---|
| g_coex_adapter_funcs_md5 | 6964e7f |
| g_coex_i154_funcs_md5 | 85dae7a |
| g_coex_basic_md5 | 7d9bf44 |

These are interface identities, not claims about the new implementation's provenance. Keep them exact when exposing the pinned compatible interface. Do not replace them with the fresh project's revision. Build revision fields `libcoexist_reversion_git/remote` can honestly identify the new project separately.

The public SDK checker computes the first seven hex characters of MD5 over the full public header file. At this specification revision the adapter header MD5 is `6964e7f2942b6edbf4fdde563c6523cb`, and the I154 header MD5 is `85dae7a9d4d8fe3284cc1d506521ce86`. The checker only schedules the I154 header test for certain other targets, but the S3 archive still exports the token/check function. Token values are additionally verified by relocation of each 4-byte pointer into its original string section: adapter offset 16, I154 offset 8, basic offset 0 in the original coexist_api.o shared string section.

The basic token is verified on **both** original libraries: `g_coex_basic_md5` points to `7d9bf44`; `g_wifi_coex_basic_md5` in `research/extracted/libnet80211/0040_ieee80211_api.o` points to the same string (string-section offset 16). Its cross-library role is public in `esp-idf/components/esp_coex/test_md5/test_md5.sh:100`. This closes the basic-token uncertainty without guessing a header that does not exist in the public SDK.

The implementation license selected by the user is MPL-2.0. This addendum records ABI and identity facts; it neither copies implementation text nor substitutes licensing claims for provenance.
