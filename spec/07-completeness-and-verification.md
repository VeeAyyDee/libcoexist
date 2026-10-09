# Closure 07: Current completeness and verification scope

The closure pass changes the feasibility conclusion: **all major missing functional contracts identified in the initial bundle now have concrete specifications suitable for fresh implementation.** They do not depend on inventing default radio policy or copying raw binary initializer arrays. The fresh implementation license selected by the user is MPL-2.0. These files record interface/behavior facts and original explanatory prose; they do not supply reconstructed C implementations.

| Earlier gap | Closure and usable result |
|---|---|
| 107 incomplete descriptor defaults | 01: every descriptor as phase count/period/named phase fields; all verified against exact exported sizes |
| Priority/duration/timer-map defaults | 01: all 42 priorities, five durations, complete event→timer behavior |
| Initial runtime data | 06: opaque core word 256, scheduler flexible period 1, log level 1, remaining zero state |
| ROM typed dispatch | 02: all twelve slot signatures and order, including retained unused release selector |
| Scheme callback mismatch risk | 02: public S3 integration confirms int callback(int), distinct from older two-argument callback type |
| Identity tokens | 02: exact adapter/I154/basic tokens, public header MD5 and cross-library checks |
| Complete policy selector precedence | 03: grouped functional rules checked against all 112 original recorded paths, zero mismatches |
| Restart and terminal phase conditions | 03: exact status-set restart condition, loop categories, phase transitions, adapter calls and callbacks |
| Hardware timer and external MMIO | 04: addresses, widths, masks, separate RMW order, barriers and argument mappings |
| Internal source-language declarations | 05: 43 normalized compatible declarations plus existing 46 public declarations = all 89 exports |
| ROM version and diagnostics | 05: ROM version is const char*; meaningful diagnostic values/order and public logging provider |

## What remains to prove in implementation

The supervisor has not read the fresh implementation trees. No claim about their behavior, compilation, tests, or license headers follows from this specification. The implementer must validate target layouts, all 206 exported symbols, weak override behavior, final linker ownership, callback typing, and host behavioral comparisons against these contracts. Diagnostic strings may identify the new implementation, but identity tokens and ABI data stay exact.

Exact unavailable private typedef *spellings* remain unrecoverable from stripped binaries. This is now an explicitly normalized C interface issue, not a missing argument-slot contract. Rich RF meaning of the two Bluetooth phase-control fields and the core opaque startup word remains unnamed, while their exact numeric representation/consumption is specified. Do not block implementation on inventing unsupported semantic labels.

True runtime concerns remain: ROM-reader quiescence during dispatch allocation/free, task-versus-ISR scheduler locking, reentrant adapter callbacks, invalid-domain historical quirks, cache-disabled memory placement, register interference and actual radio timing. These are validation/compatibility decisions, not excuses to omit specified routines. Existing reconstruction hardware receipts do not qualify a fresh implementation. No new hardware, network or upload action occurred during either specification pass.

## Verification performed on the specification itself

- Original export inventory matched exactly: 89 functions + 117 data objects, including original bindings, object sizes and sections. All inventory JSON files parsed.
- All 107 descriptor symbols decoded to the named format with exact size 2 + 4×phase_count; weights summed to 100 for each; 63 distinct schedules were observed. All 42 priorities and five duration words were accounted for.
- Public adapter and I154 header MD5 prefixes matched original required tokens; the basic token matched original coexistence and Wi-Fi libraries.
- A separately structured functional selector was evaluated against all 112 original recorded selector cases, with zero descriptor-name mismatches. That checks specification grouping against recorded observations; it is not a fresh firmware/hardware test.
- The 43 normalized internal declarations matched exactly the names lacking public SDK declarations. Together with the 46 SDK declarations, no exported function name is unaccounted for.
- MMIO transaction contracts were manually checked against the original instruction and literal dossiers, including separate reads/writes and MEMW barriers. Numeric register fields were not guessed from names.

The original nine files remain present. Numbered closures supersede their statements that exact policy defaults, full selector rules, MMIO order or usable internal declarations are unavailable. `closure-evidence-and-checks.json` records file hashes and compact verification facts for supervisor audit without embedding code or raw traces.

## Precision note for phase adapter reentrancy

Closure 03's count comparison and arm-delay period/weight use the descriptor pointer captured before `timer_disarm`. The scheduler interval is loaded later for delay computation. The descriptor pointer and index are explicitly re-read after the arm step for callback control selection. These distinct lifetimes should be preserved if a host adapter deliberately mutates scheduler state during disarm/arm. This note does not change the ordinary non-reentrant state machine. Evidence F:coex_schm_change_phase.
