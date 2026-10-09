# Closure 05: Remaining internal signatures and diagnostic dependencies

`05-internal-prototypes.json` and `.txt` supply normalized compatible declarations for all 43 exports missing from the public coexistence headers. Together with 46 public SDK declarations this accounts for all 89 exported functions. The declarations are derived from public integration signatures where available, original call argument positions, callee byte/word accesses and meaningful return paths. They are declarations only, not recovered source implementations.

## What 'normalized compatible' means

A stripped binary cannot distinguish an unavailable enum typedef from a same-width integer, a byte typedef from `uint8_t`, pointer const qualification, or a void-return function from one whose callers always ignore its residue. It is therefore impossible to honestly recover *exact original source spelling* for all 43 declarations. This is not an operational ABI blocker: the supplied normalized declarations capture every known argument slot and meaningful result, with explicit choices below. Use one consistent declaration per function across the new package and its dispatch structure.

- All `int`, enum and `uint32_t` machine slots are 32 bits on S3. Private PTI enums and wire enums can use their public SDK typedefs when visible. The normalized file uses int for those enum-valued interfaces. No hidden struct-by-value or floating-point argument was found.
- `coex_core_request` consumes a byte radio selector, full event, latency and duration. `coex_core_release` retains the first radio slot even though this body does not read it. Public Wi-Fi/Bluetooth release wrappers pass selector then event, proving the slot position.
- `ic_set_extern_coex` retains three enum-width slots although its first is unused. Public `esp_coex_external_set` forwards three arguments and consumes its int result. Thus a two-argument declaration would be wrong.
- Radio hardware timers explicitly narrow index/radio/priority to bytes; five-slot set and one-slot enable/disable signatures are also present in public controller integration declarations (Closure 02). Force/unforce use the same byte-index ABI and no meaningful result.
- `coex_core_timer_idx_get` produces only 0–3 or 255; uint8_t is the normalized index result. Callers compare against 255 before using it. `coex_core_status_get` returns a full word; `coex_is_running` is normalized bool and always produces canonical 0/1.
- `coex_schm_status_get` returns a zero-extended status word or zero; uint32_t preserves observed results. `coex_schm_status_bitmap_get` returns a word with active bits 0–4. Setter/clearer public declarations remain void.
- `coex_core_init/deinit` have bodies that explicitly return 0; normalized int retains that observation. The public deinit wrapper is still void. Core disable produces 0 or -1 internally while its public wrapper is void. Do not convert the public wrappers to int merely to expose internal results.
- Diagnostic level is a 32-bit stored bit pattern. Debug-output event/signal are enum-width values; replacing normalized int with the public debug enum typedef is safe if used consistently. Its callback signature is the public void callback declared in the existing debug header, distinct from int-returning scheduler callbacks.
- External HAL arguments are represented as full uint32_t slots. Wait-time consumes low 16 bits, GPIO consumes low three bits and each PTI consumes low four bits as described in Closure 04. Choosing a narrower C formal parameter for those exported internals is unnecessary and could change behavior of generic call harnesses without adding meaning.
- ROM data initialization/deinitialization, timer HAL and external HAL do not provide meaningful returns. Their incidental address/argument remnants in the original return register are not contracts for C clients. Preserve observable writes and calls, not a six-word execution harness signature.

Evidence per function is F:<name>. Public corroboration and ROM dispatch types are in Closure 02. The public `coex_adapter_funcs_t` retains a 64-bit `esp_timer_get_time`; do not narrow its function-pointer type to match a log's 32-bit presentation.

## ROM version and logging

Use `const char *esp_coex_rom_version_get(void)`. The original ROM-init operation invokes it with no arguments and passes its pointer directly as a string to coexistence logging. This is a revision string, not the numeric 2.0.0 API value, an integer ROM revision, or a digest. Evidence F:coex_rom_osi_funcs_init plus public S3 ROM symbol map.

`coexist_printf` is a printf-style variadic logging dependency: format pointer followed by promoted arguments; callers in this archive ignore its return. A compatible declaration `int coexist_printf(const char *format, ...);` is confirmed by the public provider declaration/definition in `esp-idf/components/esp_coex/src/lib_printf.c:44`. libc `puts`, `strlen` and `strncmp` retain their standard declarations. Preserve the existing adapter/public provider when linking firmware; a host test logger can record calls through its own mock.

Required meaningful diagnostics and call ordering:

| Operation/condition | Diagnostic contract |
|---|---|
| coex_pre_init | Log implementation build revision, then invoke core pre-init. The fresh project should identify its own revision. |
| coex_rom_osi_funcs_init | Query ROM string and log it before calling ROM data init, including when dispatch table already exists. |
| ROM dispatch allocation fails | Call adapter time getter, log allocation failure, return 0x101. |
| First adapter registration gets null | Use puts-style diagnostic for missing table; return 0x102. Existing registration bypasses this validation/log. |
| First adapter registration has wrong version/magic | Log required and supplied numeric identity; return 0x102. |
| Non-null checksum argument mismatches expected prefix | Log expected and supplied tokens; return 0x102. Null and matching cases do not emit mismatch logs. |
| Core semaphore creation fails | Call adapter time getter, log failure, return 0x101. |
| Scheduler init fails in core pre-init | Call adapter time getter, log scheduler failure, delete core semaphore, return 0x101. |
| Scheduler semaphore creation fails | Call adapter time getter, log failure, return 0x101. |
| Public status validation fails | Call adapter time getter, log invalid type/status as appropriate; return 0x102. |
| Channel getter receives either null output | Call adapter time getter, log invalid pointer arguments; return 0x102. |
| Hardware debug config rejects event or adapter call fails | Call adapter time getter, log event and error; return the error. |

Exact vendor diagnostic wording is not necessary for the user's independently written implementation. The contract preserves meaningful failures, relevant values and dependency call ordering. Existing private instruction-trace comparisons may compare exact format pointers/string bytes; such a comparison needs an explicit permitted-diagnostic normalization rather than a false claim of byte-identical output. This permission concerns diagnostics, not checksum token values or binary-interface fields. Closure 02's three interface tokens remain exact.
