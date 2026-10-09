# libcoexist

Original readable C implementation of the pinned ESP32-S3 coexistence archive
interface, licensed MPL-2.0. It preserves the `libcoexist.a` archive name, all 89
exported function names, and all 117 exported data names, including 107 distinct
weak scheduler policies. This is a source implementation of the supplied
functional contracts; it is not a ROM-free radio stack.

The implementation was written using only this tree's specification and supplied
public SDK declarations. The initial bounded milestone was expanded after
closures 01–07 supplied policy, scheduler, MMIO, startup and normalized ABI facts.
There are no unimplemented success stubs. The no-op preference and core
init/deinit routines are explicit compatibility behavior in the specification.

## Build and verification

Run `make test` with GNU C, GNU binutils and Python 3. It builds the host archive,
checks all export names/bindings and descriptor sizes, compiles 32-bit layout
assertions, and runs three ASan/UBSan executables:

- Core/API tests exercise callback typing, registration, identity prefixes,
  public status validation, lifecycle failures, wrapping enable counts,
  task/ISR locking, ROM dispatch contents/publication and deinit/reinit.
- Full integration tests exercise scheduler selection with combined flags,
  phase boundaries, timer arithmetic, reentrant callbacks, status restart
  conditions, exact MMIO transaction masks/order/barriers and external control.
- The integration suite is linked with a strong replacement for one weak policy
  and confirms that selector references resolve to the replacement.

LeakSanitizer is disabled in the command because this sandbox runs under ptrace;
AddressSanitizer and UndefinedBehaviorSanitizer remain enabled. Host doubles
exist only under `tests/`. The host archive intentionally requires simulated
MMIO callbacks and ROM-owned pointer cells from a harness. It must not be used as
a target archive. `build/export-manifest.json` records the host symbol audit.

`tools/generate_policy.py` reproducibly creates `src/policy.c` and its declarations
from the named functional fields in `spec/01-policy-defaults.json`. The generator
validates descriptor sizes, weight sums and complete priority-event coverage.
It does not read any original binary or reconstructed source. Every weak object
has independent storage; equal policies are not aliased.

The supervisor separately compared native fresh C against original Xtensa
execution: 256 selector cases and 68 MMIO cases passed, covering 314 ordered
address/width/value transactions. These are bounded independent comparisons,
reported in `spec/SUPERVISOR-FEEDBACK-02.txt`; the implementation agent did not
read their private oracle or original binary inputs. The final ESP-IDF 6.0.3
ELF audit passed for all 89 functions and 117 data exports: application ownership,
required placement, initial values, and absence of vendor-archive fallback.

Fresh two-board hardware A/B passed 984 checks across eight device boots
(original/replacement, cold/warm), with 136 verified ESP-NOW deliveries, eight
wrong-key rejections and simultaneous peer BLE advertising/scanning. Calibration
was saved to NVS and reused on both boards. Both original full flash images were
restored and verified. See `validation/results.json` for scope and receipts.
This retained vendor MAC, PHY, Bluetooth controller and WAPI implementations;
it does not establish sustained-load performance or every scheduling path.

## ESP-IDF integration

Add this directory as the `libcoexist` component and include
`cmake/replace_coexist.cmake` after the consuming project's `project()` call. The
hook redirects the pinned SDK's imported `coexist` target to the component's
`libcoexist.a`. It avoids adding an imported-target dependency cycle.

The supervisor's real SDK link check found 14 absolute ROM function assignments
that still overrode the C definitions. The included
`cmake/replace_rom_bindings.cmake` therefore generates
`libcoexist-esp32s3.rom.ld` inside the consuming build directory, omitting only
those 14 assignments. It preserves ROM data cells, other dependencies and other
linker options; the installed SDK remains unchanged. The hook requires ESP32-S3,
ESP-IDF 6.0.3 and the audited source-script SHA-256
`67b760f9ac1736bc5ab4182508ea5d3197e0908b6f2d3ae39f29fc853b9fbea9`.
It stops configuration when the pin or an expected assignment does not match.

The supervisor authored this build-only correction and the earlier absolute
`configure_file` input-path fix, recorded in
`spec/SUPERVISOR-FEEDBACK-01.txt` and `spec/SUPERVISOR-FEEDBACK-02.txt`.
Neither correction changed runtime C. Successful linking alone does not establish
that the replacement owns the intended functions; the final ELF audit is required.

The component uses real SDK configuration/system/timer declarations, compiles
`tests/layout.c` using the target compiler, and applies a whole-archive `noflash`
linker mapping. The target MMIO backend issues one Xtensa `MEMW` immediately
before every aligned 32-bit volatile read and write. Every RMW reads afresh.

The final firmware map must confirm application ownership of every intended
function and callback, correct ROM-cell resolution, complete IRAM/DRAM placement,
and absence of fallback to the vendor archive or absolute ROM function symbols.
The supervisor owns real SDK integration and separately authorized hardware
validation. Host results in this tree do not establish on-device radio timing,
concurrency correctness or complete behavioral equivalence to an original binary.

## ABI and lifecycle constraints

The native host model uses native pointers. Firmware uses actual 32-bit typed
pointers with checked adapter/core/scheduler/dispatch sizes of 80/16/64/48 bytes.
The embedded timer uses the supplied SDK ETSTimer declaration on the host and
real SDK `rom/ets_sys.h` on the target. ROM cells are extern declarations;
libcoexist never allocates replacement global cells. ROM dynamic BLE priority
and version functions remain dependencies.

The adapter table is borrowed and must remain alive. Initialization and ROM
dispatch publication/deletion require caller serialization and quiescent ROM
readers: publication precedes slot writes, and free precedes clearing the cell,
preserving the specified ordering. Scheduler locking always uses task semaphore
callbacks; it is not an ISR-lock promise.

Compatibility edge behavior is intentional. The reference count wraps at one
byte. Scheduler-init failure deletes the core semaphore without clearing its
stored address; blindly retrying that failure can reuse a dangling handle.
PTI-get requires event <42, and descriptors must have valid nonzero phase counts
and accessible phase records. Negative phase indices retain historical address
or byte behavior but are outside the supported caller domain. The code does not
turn malformed descriptors into safe schedules or silently add new range checks.

Interface tokens remain the pinned identities. Build strings identify this new
implementation. Diagnostic text is independently written; closures explicitly
permit diagnostic text normalization while preserving values and call order.

## Licensing and provenance

New C, tests, generators and build integration are MPL-2.0; see `LICENSE`.
Copied public SDK declaration files retain their Apache-2.0 notices; see
`LICENSES/Apache-2.0.txt` and `NOTICE`. Functional numeric facts are expressed in
new typed definitions, not imported opaque binary arrays. `provenance.json`
records the input hashes and scope. Historical 0BSD wording in initial spec
files is superseded by AGENTS.md and the user's MPL-2.0 selection.
