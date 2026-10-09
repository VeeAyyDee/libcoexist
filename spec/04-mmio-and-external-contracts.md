# Closure 04: Timer, debug and external coexistence hardware operations

This describes functional register transactions, not an instruction transcription. Addresses are pinned ESP32-S3 addresses. Every register read/write below is a volatile aligned 32-bit access. In the original, **each individual read and each individual write has a preceding memory-order barrier** (Xtensa MEMW). Preserve these transaction boundaries and order in a target MMIO abstraction; do not merge successive read-modify-write operations into one final write. Each named RMW reads the register afresh and preserves all bits outside its stated field. The functions perform no locking and no hardware-index range validation. Their callers own arbitration/concurrency. No hardware was used to derive this addendum.

Public S3 `soc/wdev_reg.h` corroborates this peripheral address region but does not publish the following coexistence field definitions. Numeric addresses/masks here are verified original behavior. Lack of public symbolic names is not a reason to replace the operations with no-ops.

## Radio hardware timers

Narrow timer index to its low eight bits. Its control register address C is 0x600350a8 plus eight times that index; its deadline register is C+4. Timestamp register is 0x60035000. Arithmetic is 32-bit; no saturation or range error is introduced. Valid event arbitration uses timer indices 0–3 (Closure 01), while the exported low-level routines themselves accept all byte index values.

`coex_hw_timer_set(index, radio, priority, latency, duration)` performs exactly these transactions:

1. Read C. Set bit 30 when low-byte radio is zero; clear bit 30 otherwise. Write C.
2. Re-read C. Replace bits 24–27 with the low four bits of low-byte priority. Write C.
3. Re-read C. Preserve bits 24–31 and replace bits 0–23 with low 24 bits of duration. Write C.
4. Read timestamp register 0x60035000. Add the full 32-bit latency modulo 2^32. Write the result to C+4.

This routine does not itself enable the timer. It preserves current enable/force and remaining high control bits in the RMW sequence. It does not compare the computed deadline against the current counter. The public request operation subsequently invokes enable.

`coex_hw_timer_enable` does one RMW at C, setting bit 31. Disable does one RMW clearing bit 31. Force does one RMW setting bit 28; unforce does one RMW clearing bit 28. None writes C+4. None has a meaningful return value. Evidence F:coex_hw_timer_set/enable/disable/force/unforce; literal base and timestamp constants are in the hashed original dossiers. The original's expression for C uses a scaled base; the address arithmetic above is the equivalent peripheral contract.

## Hardware debug matrix

`coex_hw_debug_matrix_config(int event)` accepts only event 0 or 1. Call the registered adapter debug_matrix_init with `(event, 33, event == 0)`. If that call returns 0, do one RMW at 0x600350a4 setting bits 0–2, then return 0. On a nonzero adapter result, log the error using adapter time and return that result. For any event other than 0/1, skip the debug-matrix callback and MMIO, log, and return 0x102.

`coex_hw_debug_matrix_init` calls config(1), then config(0), ignoring both results, and returns 0. `coex_gpio_debug_matrix_init/config` forward their corresponding results. Debug setup's low-three-bit RMW shares a register with external GPIO selection below; preserve transaction ordering rather than caching a configuration word across calls. Evidence F:coex_hw_debug_matrix_config/init and F:coex_gpio_debug_matrix_init/config.

## External coexistence HAL

`hal_coex_set_wait_time(first, second)` writes the high 16 bits of register 0x60035098 from low 16 bits of first, preserving the low 16 bits. Then it performs the corresponding RMW at 0x6003509c from second. Inputs are not interpreted as signed values or clamped. Each register has its own read and write.

`hal_enable_extern_coex`: first RMW 0x60033c6c clearing bit 26; then RMW 0x60035084 setting bit 0. `hal_disable_extern_coex` uses the same register order, setting bit 26 in the first and clearing bit 0 in the second.

`hal_set_extern_coex_gpio(selection)` performs one RMW of 0x600350a4, replacing only bits 0–2 with low three bits of selection.

`hal_set_extern_pti(p0,p1,p2,p3,p4,p5)` first calls `hal_coex_set_wait_time(2,2)`. Then it performs **seven separate RMWs** at register 0x60035120, in this exact order:

| Transaction | Field replaced | New field value |
|---:|---|---|
| 1 | bits 12–15 | zero |
| 2 | bits 8–11 | low four bits of p1 |
| 3 | bits 4–7 | low four bits of p0 |
| 4 | bits 0–3 | low four bits of p2 |
| 5 | bits 24–27 | low four bits of p3 |
| 6 | bits 20–23 | low four bits of p4 |
| 7 | bits 16–19 | low four bits of p5 |

All other bits are preserved at each step. These six arguments are full 32-bit machine slots; only their stated low nibbles affect hardware.

`hal_clr_extern_pti` performs four separate RMWs at 0x60035120, clearing bits 12–15, then 8–11, then 4–7, then 0–3. It deliberately leaves bits 16–31 unchanged, including the high PTI fields set by `hal_set_extern_pti`. None of these HAL routines has a meaningful return value. Evidence F:each matching HAL function; address/mask literals and original access order recorded in the hashed dossiers.

## External start/stop mapping

`ic_set_extern_coex(level1, level2, level3)` preserves the public three-slot ABI although level1 is unused. It calls core enable and ignores its return, then sets external status bit 1 (type 3, input 1). Derive a priority from level2 and another from level3 independently: input exactly 1 → 15, input exactly 0 → 11, every other full 32-bit input → 3. Configure external GPIO selection 7. Call `hal_set_extern_pti(3, derived_level2, derived_level3, 0, 0, 0)`. Enable external hardware. Return 0.

`ic_stop_extern_coex` clears external status bit 2, clears external status bit 1, calls core disable, clears external PTI, then disables external hardware, in that order. It has no meaningful result. Public external set/stop are direct wrappers; public txline toggles external bit 2. `esp_coex_external_set_wire_type/get_wire_type` write/read the private full 32-bit wire value independently of these MMIO functions.

Evidence F:ic_set_extern_coex/ic_stop_extern_coex and public external wrappers. The public external PTI enum has a smaller named domain; preserve the stated machine-level fallback when testing non-enum inputs. These contracts close the previous unspecified MMIO/default-mapping gaps. On-device timing and concurrent register interference remain qualification tasks, not missing operation specifications.
