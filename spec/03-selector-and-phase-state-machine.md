# Closure 03: Complete selector precedence and phase state machine

Status variables in this document are unsigned 16-bit Wi-Fi W, BLE L, classic Bluetooth T, external E and I154 I, at the offsets already specified. A mask test means at least one indicated bit is set. Apply decisions in the stated order, stopping at the first result. Preserve the exported descriptor pointer itself, including symbol identity; some equally valued schedules have different names.

## Descriptor selection

Begin by storing `coex_schm_all_default` as the active descriptor. The rules below may then replace it; selection never changes phase index, disarms/arms a timer, or invokes callbacks itself.

1. If E has bit 0 set, choose external coexistence immediately. W bit 0 selects `coex_schm_external_coex_wifi_scan`; otherwise W bit 1 selects `coex_schm_external_coex_wifi_connecting`; otherwise E bit 1 selects `coex_schm_external_coex_wifi_default_rxonly`; otherwise choose `coex_schm_external_coex_wifi_default`. All other status values are irrelevant on this branch.
2. For all remaining branches determine the Wi-Fi suffix with precedence: W bit 0 → `scan`; otherwise W bit 1 → `connecting`; otherwise W bit 2 → `conn`. If none is set, leave `coex_schm_all_default`. Bits beyond bit 2 do not supply a suffix.
3. If both L and I are zero, use the classic-Bluetooth-only family described below.
4. Otherwise, select the BLE family in this precedence: L bit 3 → mesh `config`; otherwise L bit 4 → mesh `traffic`; otherwise L masked by 0x3a equals exactly 2 → mesh `traffic`; otherwise L bit 5 → mesh `standby`; otherwise use `ble_default`.

The second traffic condition is deliberate: a BLE state with bit 1 set and bits 3,4,5 clear is treated as mesh traffic even though its name may not suggest it. Unexamined higher bits do not defeat that masked equality. I154 activity takes the selector out of the classic-BT-only family even if L is zero.

### Classic-Bluetooth-only family

A zero T chooses `coex_schm_bt_idle_wifi_<suffix>`. For nonzero T, use this ordered table; a mask match picks the category and ends the search.

| Precedence | scan and conn suffixes | connecting suffix |
|---:|---|---|
| 1 | mask 0x40 → page | mask 0x20 → a2dp_paused |
| 2 | mask 0x02 → inq | mask 0x10 → a2dp |
| 3 | mask 0x20 → a2dp_paused | mask 0x40 → page |
| 4 | mask 0x10 → a2dp | mask 0x02 → inq |
| 5 | mask 0x88 → sniff_sco | mask 0x88 → sniff_sco |
| 6 | mask 0x04 → conn | mask 0x04 → conn |
| 7 | mask 0x01 → piscan | mask 0x01 → piscan |
| fallback | default | default |

The complete name is `coex_schm_bt_<category>_wifi_<suffix>`. This distinction between connecting and scan/conn precedence matters when T contains multiple bits.

### Mesh config/traffic/standby families

A zero T chooses `coex_schm_ble_mesh_<family>_wifi_<suffix>` without a BT category. For nonzero T, choose the first matching category: 0x20 → a2dp_paused; 0x10 → a2dp; 0x88 → sniff_sco; 0x04 → conn; 0x01 → piscan; otherwise default. The name is `coex_schm_ble_mesh_<family>_bt_<category>_wifi_<suffix>`. This precedence is the same for all three Wi-Fi suffixes and all three mesh families. Page/inquiry-only T therefore uses the default category here.

### Non-mesh BLE default family

Zero T chooses idle; otherwise T bit 4 chooses a2dp; otherwise choose default. The name is `coex_schm_ble_default_bt_<category>_wifi_<suffix>`. The original has an extra branch for L=0, T=0, I=1, but its resulting descriptor is exactly the same idle choice and has no other writes/calls; it need not appear in an independently structured selector.

The three exported policies ending in `wifi_default` are not selected by this pinned selector: `coex_schm_ble_default_bt_a2dp_wifi_default`, `coex_schm_ble_default_bt_idle_wifi_default`, and `coex_schm_ble_idle_bt_idle_wifi_default`. They remain required weak exports and their numeric defaults are in Closure 01. No-Wi-Fi-bit selection remains `coex_schm_all_default`.

Evidence: F:coex_schm_status_change. Supervisor checked the grouped functional rules against all 112 recorded original selector paths, including every final descriptor store in those paths: zero mismatches. Those 112 paths exercise every instruction and recorded branch/conditional-move item in the pilot dossier, but are not a new runtime test of a fresh implementation. Full 16-bit spaces can be tested through equivalence classes of the masks/zero tests above.

## Set/clear status and restart predicate

Both operations first acquire scheduler lock; nonzero lock result returns immediately. Unsupported unsigned type ≥5 changes nothing and still unlocks. Types 0,1,2,3,4 map to W,L,T,E,I respectively.

Set records loop eligibility before the mutation. For a valid type, compute whether any requested 32-bit input bit is absent from the zero-extended old 16-bit field. If yes, store the low 16 bits of old OR input and mark a change; otherwise mark no change. Consequently high input bits can mark a change even though the stored 16-bit field is unchanged. Recompute loop eligibility after the write. When marked changed, select the descriptor under the same lock. Release the lock. Restart phase processing with restart=true **only when all four conditions hold**: marked changed, old loop eligibility false, new loop eligibility true, and the current W (re-read after unlock) equals exactly 1, 2 or 64. Do not replace this with restarting after every policy change.

Clear checks whether any requested bit is present in the zero-extended old 16-bit field. On a match, store low 16 bits of old AND complement(input), then select the descriptor under lock. On no match, skip both store and selector. Unlock. Clear does not invoke phase restart. High-only input bits cannot match an old 16-bit field.

Loop eligibility counts four radio groups: W nonzero; (L OR T) nonzero; E nonzero; I nonzero. Return true when at least two groups are active. BLE+BT count as one group. Evidence F:coex_schm_status_bit_set/clear and F:coex_schm_is_loop_allowed.

## Exact phase-change algorithm

The internal phase-change operation takes a restart flag; timeout passes false, public restart passes true. A successful operation returns 0; failure returns -1. Existing public `coex_schm_process_restart` propagates that result.

Acquire scheduler lock. On lock failure return -1 immediately, without debug callback. Read the active descriptor pointer. Null pointer leads to failure handling below.

With restart=false, if current byte index is at least phase_count minus 1 (the subtraction/comparison is signed integer arithmetic), inspect W. If W is not exactly 1,2,64, fail. If it is one of those values but loop eligibility is false, fail. Otherwise treat this transition as a restart. With restart=true, skip these terminal checks. A count of zero therefore does not acquire a special safe meaning: malformed custom descriptors remain outside the valid descriptor contract.

For a transition that proceeds:

1. Call adapter timer_disarm on the embedded timer while holding scheduler lock.
2. Set byte index to zero for a restart; otherwise increment the old byte index with byte wraparound.
3. If the new index is not phase_count minus 1, arm the one-shot timer. At the last phase, arm only when W is exactly 1,2,64 **and** loop eligibility is true. Otherwise leave it disarmed.
4. The arm delay is the low 32 bits of descriptor period × current interval × selected phase duration weight. The adapter call receives repeat=false. Timer registration uses its SDK ABI; this timer is distinct from radio hardware timer registers.
5. After any arm call, re-read the active descriptor and index, then select callback pointers using that phase's controls. Nonzero Wi-Fi control selects the current Wi-Fi callback; either Bluetooth control nonzero selects current BT callback. Capture these pointers while locked. Do not use a cached descriptor if a reentrant adapter changes published state.
6. Unlock. Emit debug output event 4, signal 2. Invoke captured non-null Wi-Fi callback with int event 0, then captured non-null BT callback with int event 0. Ignore their int returns. Return 0.

Failure after acquiring the lock unlocks, emits the same debug event, invokes no scheme callbacks, and returns -1. Timer disarm is not performed in those failure paths. Evidence F:coex_schm_change_phase, F:coex_schm_timeout_process, F:coex_schm_process_restart. Read-after-adapter-call details matter because the adapter function is a real callback, not pure arithmetic.

These rules close the earlier generic selector/phase prerequisites for an implementation. Real-time scheduling, ISR legality and concurrent adapter/ROM consumers remain validation concerns, not missing numeric policy specifications.
