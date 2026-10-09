# Policy facts, original design choices and remaining work

## Descriptor ABI and what the data mean

A scheduler descriptor has a two-byte header and a variable number of four-byte phase records. Header byte 0 is phase count; header byte 1 is the period multiplier returned by `coex_schm_curr_period_get`. Each phase contains, in order, duration weight, Wi-Fi control, Bluetooth-control field A and Bluetooth-control field B. These field names describe established consumers; the last two fields' exact division into BLE/classic-BT event masks remains unresolved. They are **not booleans**. Record address is header plus four times phase index. Timer delay is interval multiplied by period multiplied by duration weight. The callback gating behavior is specified separately.

The 107 weak symbol sizes agree with phase counts 1, 2, 3 or 5 (total sizes 6, 10, 14 or 22). All inspected original descriptors have positive weights summing to 100; 63 distinct complete schedules exist across the 107 names. There is no pointer relocation inside these phase descriptors. They are immutable policy inputs as supplied, with weak binding to permit link-time policy override. Preserving weak binding and object size is essential even where multiple names have equal values; aliasing or deduplicating distinct objects may change pointer identity and requires justification.

A few verified behavioral examples, expressed as fields rather than opaque byte dumps:

| Policy symbol | Period | Phase duration weights in order | Corresponding controls |
|---|---:|---|---|
| coex_schm_all_default | 1 | 40, 60 | First phase Wi-Fi control 3; all other control fields zero |
| coex_schm_bt_idle_wifi_scan | 12 | 100 | Wi-Fi control 2; both Bluetooth controls zero |
| coex_schm_bt_idle_wifi_connecting | 10 | 100 | Wi-Fi control 2; both Bluetooth controls zero |
| coex_schm_bt_idle_wifi_conn | 1 | 100 | Wi-Fi control 2; both Bluetooth controls zero |
| coex_schm_ble_idle_bt_idle_wifi_default | 1 | 100 | All controls zero |
| coex_schm_ble_mesh_config_bt_conn_wifi_conn | 1 | 25, 20, 55 | Phase 1: Wi-Fi 3, A/B zero. Phase 2: Wi-Fi zero, A 32, B 4. Phase 3: Wi-Fi zero, A 2, B 64. |

Evidence D:each named symbol, with decoded format independently corroborated by F:coex_schm_curr_period_get, F:coex_schm_get_phase_by_idx and F:coex_schm_change_phase. These examples do not specify the other 101 initializers. Do not fill their missing values by extrapolating a name or by assigning all-default to every symbol. A semantic table of independently audited factual field values could be supplied in a later bounded specification; that is distinct from wholesale copying a binary blob into an 'original' package.

The PTI export has 42 priorities and setter accepts values 0–15. Its defaults are arbitration policy rather than calibration. The five duration parameters are functional timing policy. Neither should be renamed 'calibration' to justify leaving a proprietary table in an otherwise original-source archive. Exact default values and event-to-timer lookup facts can be specified incrementally after verification; no full defaults are claimed here.

## Selection facts and unresolved precedence

The private selector always starts from the all-default descriptor, then examines Wi-Fi, BLE, classic BT, external and I154 status. A selected descriptor pointer is published in scheduler state; selection itself does not directly invoke callbacks or program the timer.

The external enabled bit has precedence over the ordinary Wi-Fi/Bluetooth policy family. With it set, Wi-Fi scan selects external-scan, then Wi-Fi connecting selects external-connecting; otherwise external receive-only bit 2 selects its receive-only default, and the external default is the remaining case. The scan/connecting predicates examine Wi-Fi status bits 0/1 in that order. Evidence F:coex_schm_status_change. This is a behavior fact, not a complete generic coexistence design.

Other families cover classic Bluetooth idle/page/inquiry/A2DP paused/A2DP/SCO-or-sniff/connected/page-inquiry-scan/default crossed with Wi-Fi scan/connecting/connected, BLE mesh config/traffic/standby, and non-mesh BLE defaults. The original selector has 465 instructions and many overlapping-bit priority branches. The symbol names alone do **not** establish all winner precedence. Full semantic truth tables need to include multiple simultaneous Bluetooth bits, BLE mesh flags, I154 activity, no Wi-Fi state, and out-of-enum status bits. Matching each isolated named state is insufficient.

## Concrete implementation milestones

1. **ABI shell and focused host core.** A fresh agent reads only this specification and public SDK declarations, creates original typed state/adapter interfaces, verifies target sizes/offsets and implements checksum prefix checks, adapter registration, public validation wrappers, version metadata, debug/lpclk callbacks, simple channel/interval/flexible-period accessors, and lock adapters. Provide injectable adapter state and policy inputs in host tests. Missing scheduler/hardware functions must remain visibly unimplemented, not quiet stubs advertised as a full replacement. This deliverable is a bounded library subset, not a firmware drop-in archive.
2. **Lifecycle and ROM bridge.** Add core semaphore lifecycle, wrapping reference count, callback sequencing and all ROM pointer/dispatch operations. Host tests cover first/repeated initialization, allocation failure, existing dispatch pointer, deinit/reinit, ISR wake behavior and non-1 semaphore results. Target build checks must verify 80/16/64/48-byte layouts, pointer indirection, IRAM reachability and linker-map ownership. Resolve caller serialization and ROM-reader quiescence before runtime use.
3. **Policy specification closure.** Supervisor supplies factual semantic fields for every default descriptor and the PTI/duration/event-timer tables, with field interpretations, aliases, precedence and unresolved flags marked honestly. Fresh agent expresses those contracts with original named policy structures or a generator designed from those contracts. Preserve independent provenance; do not import raw arrays, generated native_objects code, instruction C or a register-machine interpreter. If a new scheduling policy is chosen instead, document measurable differences and do not call it vendor-equivalent.
4. **Scheduler closure.** Independently implement descriptor selection and phase state machine after the complete status/restart/terminal truth tables exist. Test combined state flags, loop eligibility, phase count boundaries, forced restart versus timeout, empty descriptor cases, 32-bit delay multiplication, high input bits, callbacks after unlocking, reentrant callbacks and weak overrides. Distinguish invalid-domain quirks from intended valid-domain compatibility. No hidden 'fix' should be called exact equivalence.
5. **Hardware contract closure.** Extract and verify timer MMIO, hardware debug routing and external coexistence register contracts against pinned public S3 register definitions and private original observations. Use a simulated MMIO backend to compare access widths/masks/order for valid indices. A semantic model must account for counter wrap and concurrent register owners. Building successfully is not validation of on-device timing.
6. **Whole-archive qualification.** Check all 89 function exports and 117 object exports, their bindings/sizes/placements, weak override behavior, complete undefined-symbol inventory, and final firmware map callback ownership. Supervisor privately compares the original and fresh implementations with the same declared fixtures. Record which original traces remain bounded, unsupported or newly covered. Existing reconstruction tests cannot be re-labeled as fresh implementation results. Hardware and radio validation require separately authorized work; none is included in this specification task.

The first two milestones are practical incremental work. Milestones 3–5 remain release gates, not reasons to generate plausible-looking success stubs. They do not require a promise to rewrite 95 bodies at once.

## Specific completion blockers

- Forty-three internal exports have no public SDK prototype identified. ELF gives names/binding, not source types. Resolve used/unused parameter positions, width, signedness and meaningful versus void returns from callers before publishing a universal header. `ic_set_extern_coex`, `coex_core_release` and the six-argument external HAL are concrete traps.
- Exact defaults are only partially expressed as semantic facts here. No evidence shows a huge irreducible RF calibration blob, but 107 descriptor policies, PTI and duration defaults cannot be omitted from strict behavior compatibility.
- Complete selector precedence and status-set restart conditions are not yet independently specified. Phase lifecycle contracts are substantially understood but full terminal/index edge behavior needs closure.
- ROM dispatch publication precedes slot initialization in the original; deinit frees before clearing pointer. Caller serialization and downstream ROM use must be established. A fully original source package may retain documented chip-ROM dependencies; a ROM-free claim is false for the described integration.
- Repeated pre-init after allocation/setup failure, dangling semaphore storage, counter wrap/underflow, negative phase indices and getter out-of-range reads need explicit compatibility versus hardening decisions. Passing a convenient happy path does not settle them.
- Final IRAM/DRAM and linker symbol selection can invalidate a source-level implementation even with matching names. MMIO/timing/concurrency have no new hardware evidence in this task.

The deliverable is an honest bounded specification sufficient to start fresh implementations and identify what must still be learned. It is not a complete drop-in or licensing certification. Original source can be offered under 0BSD when independently written with appropriate provenance; copied vendor tables/source and copied Apache-licensed SDK implementation would need their own applicable treatment. Functional interface facts and original policy algorithms do not by themselves require reusing vendor implementation text.
