# Closure 06: Exact initial runtime state

This is separate from Closure 01 so that the previously pinned default-policy JSON and Markdown remain unchanged. It records functional startup values from original storage, not new defaults invented for the implementation.

| Object/field | Initial value | Evidence |
|---|---|---|
| coex_env semaphore pointer at offset 0 | null | D:coex_env |
| coex_env status word at offset 4 | 0 | D:coex_env |
| coex_env opaque word at offset 8 | **256 (0x100)** | D:coex_env |
| coex_env start callback at offset 12 | null | D:coex_env |
| coex_schm_env flexible period byte at offset 58 | **1** | D:coex_schm_env |
| All other coex_schm_env fields and padding | zero, including initial descriptor pointer | D:coex_schm_env |
| coex_log_level | **1** | D:coex_log_level |
| Private coex_enabled_ref | 0, one byte | coexist_core.o `.bss.coex_enabled_ref`, size 1, NOBITS |
| Private g_external_coex_wire_type | 0, four bytes | external_coex.o `.bss.g_external_coex_wire_type`, size 4, NOBITS |
| Private low-power clock callback | null | coexist_api.o `.bss.s_set_lpclk_source_cb`, NOBITS |
| Private ROM data initialization guard | false/0 | coexist_param.o `.bss.s_coex_rom_data_init`, NOBITS |

Do not zero-initialize the entire core object and lose the opaque offset-8 word. Its richer semantic name is unresolved; exact numeric representation is sufficient for ABI fidelity. The scheduler's null initial descriptor is replaced by `coex_schm_init` with the all-default descriptor; this does not initialize the interval to an invented nonzero value.

The parent's independent inspection confirmed the offset-8, flexible-period, log-level and private zero-initialization facts. Existing `evidence-index.json` maps D symbols to original ELF objects/sections. PTI, event duration and weak schedule defaults remain in unchanged Closure 01.
