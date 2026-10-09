/* SPDX-License-Identifier: MPL-2.0 */
#include "coexist_subset.h"
#include "coexist_mmio.h"
#include "coexist_diagnostics.h"
#define IRAM __attribute__((section(".coexiram")))
static IRAM void replace_field(uint32_t address, uint32_t mask, uint32_t value)
{ coex_write32(address, (coex_read32(address) & ~mask) | (value & mask)); }
static IRAM uint32_t timer_address(uint8_t index) { return 0x600350a8u + 8u * index; }
IRAM void coex_hw_timer_set(uint8_t index, uint8_t radio, uint8_t priority, uint32_t latency, uint32_t duration)
{
    uint32_t address = timer_address(index);
    replace_field(address, 1u << 30, radio == 0 ? 1u << 30 : 0);
    replace_field(address, 15u << 24, (uint32_t)priority << 24);
    replace_field(address, 0xffffff, duration);
    uint32_t deadline = coex_read32(0x60035000) + latency;
    coex_write32(address + 4, deadline);
}
IRAM void coex_hw_timer_enable(uint8_t index) { replace_field(timer_address(index), 1u << 31, 1u << 31); }
IRAM void coex_hw_timer_disable(uint8_t index) { replace_field(timer_address(index), 1u << 31, 0); }
IRAM void coex_hw_timer_force(uint8_t index) { replace_field(timer_address(index), 1u << 28, 1u << 28); }
IRAM void coex_hw_timer_unforce(uint8_t index) { replace_field(timer_address(index), 1u << 28, 0); }
int coex_hw_debug_matrix_config(int event)
{
    if (event != 0 && event != 1) { COEX_DIAG("invalid debug event=%d", event); return ESP_ERR_INVALID_ARG; }
    int result = g_coa_funcs_p->_debug_matrix_init(event, 33, event == 0);
    if (!result) replace_field(0x600350a4, 7, 7);
    else COEX_DIAG("debug event=%d failed: %d", event, result);
    return result;
}
int coex_hw_debug_matrix_init(void)
{
    (void)coex_hw_debug_matrix_config(1);
    (void)coex_hw_debug_matrix_config(0);
    return 0;
}
esp_err_t coex_gpio_debug_matrix_init(void) { return coex_hw_debug_matrix_init(); }
esp_err_t coex_gpio_debug_matrix_config(int event) { return coex_hw_debug_matrix_config(event); }
void hal_coex_set_wait_time(uint32_t first, uint32_t second)
{
    replace_field(0x60035098, 0xffff0000, first << 16);
    replace_field(0x6003509c, 0xffff0000, second << 16);
}
void hal_enable_extern_coex(void)
{
    replace_field(0x60033c6c, 1u << 26, 0);
    replace_field(0x60035084, 1, 1);
}
void hal_disable_extern_coex(void)
{
    replace_field(0x60033c6c, 1u << 26, 1u << 26);
    replace_field(0x60035084, 1, 0);
}
void hal_set_extern_coex_gpio(uint32_t selection) { replace_field(0x600350a4, 7, selection); }
void hal_set_extern_pti(uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5)
{
    hal_coex_set_wait_time(2, 2);
    replace_field(0x60035120, 15u << 12, 0);
    replace_field(0x60035120, 15u << 8, p1 << 8);
    replace_field(0x60035120, 15u << 4, p0 << 4);
    replace_field(0x60035120, 15, p2);
    replace_field(0x60035120, 15u << 24, p3 << 24);
    replace_field(0x60035120, 15u << 20, p4 << 20);
    replace_field(0x60035120, 15u << 16, p5 << 16);
}
void hal_clr_extern_pti(void)
{
    replace_field(0x60035120, 15u << 12, 0);
    replace_field(0x60035120, 15u << 8, 0);
    replace_field(0x60035120, 15u << 4, 0);
    replace_field(0x60035120, 15, 0);
}
