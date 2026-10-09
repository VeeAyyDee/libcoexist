# Closure 01: Complete functional default policy records

`01-policy-defaults.json` supplies every one of the 107 weak descriptor defaults as named semantic fields, all 42 event priorities grouped by equal value, all five event durations, and the complete event-to-hardware-timer mapping. No vendor source or opaque byte-array initializer is included. The numeric fields are behavioral ABI facts; a fresh implementation should express them through its own typed policy model.

A descriptor phase contains a duration weight, Wi-Fi control and two explicitly opaque Bluetooth control fields. The current scheduler gates the Wi-Fi callback on nonzero Wi-Fi control, and gates the BT callback on either Bluetooth field being nonzero. Do not normalize those fields to booleans: downstream readers observe values such as 2, 3, 4, 32 and 64. Unknown richer radio meaning does not prevent exact representation of the verified numbers.

Each phase delay is interval times period multiplier times duration weight, in 32-bit arithmetic. All supplied descriptor weights sum to 100. Preserve each exported symbol's size and weak binding, even when schedules are identical. Do not assume identical contents permit pointer aliasing.

Evidence: D:<symbol> in the original evidence index; PTI/durations use D:coex_pti_tab and D:g_coex_param. Event mapping uses F:coex_core_timer_idx_get and the original coexist_core.o private section `.rodata.CSWTCH$26`. Original section bytes were decoded privately; this addendum publishes structured field facts, not raw sections. Phase field interpretation is corroborated by F:coex_schm_change_phase and F:coex_schm_get_phase_by_idx.

Validation performed during specification: all 107 names matched the export inventory, all lengths matched 2 + 4 times phase count, every phase-weight sum was 100, 42 priority entries were accounted for, and all five 32-bit duration words were decoded little-endian. This validates transcription and schema; runtime equivalence remains an implementation test.
