# Fresh libcoexist: bounded specification and feasibility

This is a supervisor-authored, implementation-free specification for the pinned ESP32-S3 libcoexist archive in this workspace. It contains ABI facts and prose behavioral observations, not decompiler output, translated assembly, original policy initializers, or an implementation. A fresh implementer may read this directory and the named public SDK headers. They must not follow supervisor-private evidence links into original objects, reconstructed code, dossiers or traces. This document is not a claim that a clean replacement already exists.

The exact archive interface is **89 exported functions and 117 exported data objects**. Six additional private routines implement core locking and scheduler internals. All 89 function names/bindings/sections and 117 data names/sizes/bindings are listed in `function-exports.json` and `data-exports.json`. The public SDK provides declarations for 46 of those 89 names. The other 43 source-language prototypes are not fully recoverable from ELF symbol metadata; their observed argument slots are explicitly lower bounds. Do not turn a six-register execution harness into a public C prototype or treat residual register values from void functions as meaningful returns.

The 117 exported data objects occupy 1,396 bytes: 107 weak scheduler descriptors total 1,230 bytes; the remaining ten objects total 166 bytes. Thus there is **no large calibration blob among these exported data objects**. There is still substantive vendor scheduling policy: phase weights and radio flags in 107 descriptors, a 42-byte event-priority table and five duration parameters. Descriptors are a policy family, not executable algorithms. Their raw values are deliberately absent. Additional private literals/jump tables/strings are not included in the exported-data byte count.

A strict source replacement is feasible in stages. Interface validation, callback registration, simple state accessors, lock wrappers and ROM publication are small enough to specify precisely. Exact scheduling, timer MMIO, weak-policy defaults and their interaction with ROM require further contract work and independent verification. Replacing descriptors with invented 'balanced' defaults could yield an original source package, but would be a behavioral change, not established compatibility. A complete drop-in claim is premature. Copying opaque vendor arrays into C and attaching a new license is not an honest original-source solution.

Files:

- `abi-and-rom.md`: layouts, adapter slots, ROM-owned pointers, publication/lifetime and linker hazards.
- `behavior-contracts.md`: concrete contracts and bounded observations grouped by responsibility.
- `policy-and-plan.md`: policy format/selection facts, remaining blockers and implementable milestones.
- `public-declarations.txt`: declaration references from public SDK source (use SDK headers and preserve their licenses).
- `function-exports.json`, `data-exports.json`, `external-dependencies.json`: complete symbol inventories, no initializer bytes.
- `evidence-index.json`: supervisor-private evidence locations and hashes; not implementation input beyond identifying a claim.

Evidence notation `F:name` names a original-body dossier record in the evidence index. `D:name` names the original ELF symbol identified by the data inventory. These references allow a supervisor to audit facts without exposing original code to the implementer. Machine traces use stubbed callees and do not themselves establish API types or real concurrency correctness.

Existing reconstruction receipts report 95 bodies, 89 executed bodies, 1,485/2,141 instructions, 313 matching fixtures and 11 bounded/unsupported cases, with no differences. These are results for the earlier instruction-derived reconstruction, **not** for any future fresh implementation. Later pilot dossiers have different coverage and must not be conflated with that receipt. Historical radio tests also belong to that reconstruction. This specification session performed no hardware actions, networking, uploads or cleanroom implementation inspection.
