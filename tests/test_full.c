/* SPDX-License-Identifier: MPL-2.0 */
#include "coexist_subset.h"
#include "coexist_diagnostics.h"
#include "coexist_policy.h"
#include "coexist_mmio.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
coex_adapter_funcs_t *g_coa_funcs_p;
coex_core_state *coex_env_ptr;
coex_scheduler_state *coex_schm_env_ptr;
uint8_t *coex_pti_tab_ptr;
uint32_t *g_coex_param_ptr;
coex_rom_dispatch *coexist_funcs;
static int semaphores[2], created, held, take_result = 1;
static int disarms, arms, setups, done_calls, debug_calls, wifi_calls, bt_calls;
static int matrix_calls, matrix_result, create_fail, replace_on_arm;
static uint32_t delay;
static ETSTimerFunc *timeout_handler;
static char callbacks[16];
static unsigned callback_count;
static coex_schedule combined = { .phase_count = 2, .period = 13, .phases = {
    { .weight = 25, .wifi_control = 3, .bt_control_a = 2 },
    { .weight = 75, .bt_control_b = 64 }
}};
static void *create(uint32_t maximum, uint32_t initial)
{ assert(maximum == 1 && initial == 1); return create_fail ? NULL : &semaphores[created++ % 2]; }
static int32_t take(void *handle, uint32_t ticks)
{ assert(handle && ticks == UINT32_MAX); if (take_result == 1) { assert(!held); held = 1; } return take_result; }
static int32_t give(void *handle) { assert(handle && held); held = 0; return 1; }
static int in_isr(void) { return 0; }
static void disarm(void *timer) { assert(held && timer == &coex_schm_env.timer); ++disarms; }
static void arm(void *timer, uint32_t us, bool repeat)
{
    assert(held && timer == &coex_schm_env.timer && !repeat); ++arms; delay = us;
    if (replace_on_arm) { coex_schm_env.current = &combined; coex_schm_env.phase_index = 0; }
}
static void setfn(void *timer, void *function, void *argument)
{ assert(timer == &coex_schm_env.timer && !argument); timeout_handler = (ETSTimerFunc *)function; ++setups; }
static void done(void *timer) { assert(timer == &coex_schm_env.timer); ++done_calls; }
static int matrix(int event, int signal, bool reverse)
{ assert((event == 0 || event == 1) && signal == 33 && reverse == (event == 0)); ++matrix_calls; return matrix_result; }
static void debug(int event, coex_gpio_debug_sig_t signal)
{
    assert(!held && event == 4 && (int)signal == 2); ++debug_calls;
    if (callback_count < sizeof(callbacks)) callbacks[callback_count++] = 'D';
}
static int wifi(int event)
{
    assert(!held && !event); ++wifi_calls;
    if (callback_count < sizeof(callbacks)) callbacks[callback_count++] = 'W';
    assert(coex_schm_interval_get() == coex_schm_env.interval);
    return -5;
}
static int bt(int event)
{ assert(!held && !event); ++bt_calls; if (callback_count < sizeof(callbacks)) callbacks[callback_count++] = 'B'; return 3; }
int coex_core_ble_conn_dyn_prio_get(bool *low, bool *high) { *low = false; *high = false; return 0; }
const char *esp_coex_rom_version_get(void) { return "host-rom"; }
static int64_t get_time(void) { return INT64_C(0x100000001); }
int coexist_printf(const char *format, ...) { assert(format); return 0; }
static coex_adapter_funcs_t adapter = {
    ._esp_timer_get_time = get_time,
    ._version = 2, ._magic = (int32_t)COEX_ADAPTER_MAGIC,
    ._semphr_create = create, ._semphr_take = take, ._semphr_give = give, ._is_in_isr = in_isr,
    ._timer_disarm = disarm, ._timer_arm_us = arm, ._timer_setfn = setfn, ._timer_done = done,
    ._debug_matrix_init = matrix, ._malloc_internal = malloc, ._free = free
};
/* Register file and ordered bus trace. Each access must consume its own barrier. */
static uint32_t registers[0x600];
struct transaction { char kind; uint32_t address, value; };
static struct transaction trace[128];
static unsigned transaction_count, barriers;
static unsigned register_index(uint32_t address)
{ assert(!(address & 3) && address >= 0x60033c00 && address < 0x60035400); return (address - 0x60033c00) / 4; }
void coex_test_barrier(void) { ++barriers; }
uint32_t coex_test_read32(uint32_t address)
{
    assert(barriers == transaction_count + 1 && transaction_count < 128);
    uint32_t value = registers[register_index(address)];
    trace[transaction_count++] = (struct transaction){'R', address, value}; return value;
}
void coex_test_write32(uint32_t address, uint32_t value)
{
    assert(barriers == transaction_count + 1 && transaction_count < 128);
    trace[transaction_count++] = (struct transaction){'W', address, value}; registers[register_index(address)] = value;
}
static void clear_trace(void) { transaction_count = barriers = 0; }
static void expect(unsigned n, char kind, uint32_t address, uint32_t value)
{ assert(n < transaction_count && trace[n].kind == kind && trace[n].address == address && trace[n].value == value); }
static void test_mmio(void)
{
    const uint32_t control = 0x600350b8;
    registers[register_index(control)] = 0xb2345678;
    registers[register_index(0x60035000)] = UINT32_MAX - 5;
    coex_hw_timer_set(2, 0, 0xa5, 10, 0x12345678);
    assert(transaction_count == 8 && barriers == 8);
    expect(0, 'R', control, 0xb2345678); expect(1, 'W', control, 0xf2345678);
    expect(2, 'R', control, 0xf2345678); expect(3, 'W', control, 0xf5345678);
    expect(4, 'R', control, 0xf5345678); expect(5, 'W', control, 0xf5345678);
    expect(6, 'R', 0x60035000, UINT32_MAX - 5); expect(7, 'W', control + 4, 4);
    clear_trace(); coex_hw_timer_set(2, 255, 255, 0, 0xffffffff);
    expect(1, 'W', control, 0xb5345678); expect(3, 'W', control, 0xbf345678); expect(5, 'W', control, 0xbfffffff);
    clear_trace(); coex_hw_timer_disable(2); coex_hw_timer_unforce(2); coex_hw_timer_enable(2); coex_hw_timer_force(2);
    assert(transaction_count == 8); expect(1, 'W', control, 0x3fffffff); expect(3, 'W', control, 0x2fffffff);
    expect(5, 'W', control, 0xafffffff); expect(7, 'W', control, 0xbfffffff);
    clear_trace(); registers[register_index(0x60035098)] = 0xaaaabbbb; registers[register_index(0x6003509c)] = 0xccccdddd;
    hal_coex_set_wait_time(0x12345678, 0x87654321);
    expect(1, 'W', 0x60035098, 0x5678bbbb); expect(3, 'W', 0x6003509c, 0x4321dddd);
    clear_trace(); registers[register_index(0x60035120)] = UINT32_MAX;
    hal_set_extern_pti(0x11, 0x22, 0x33, 0x44, 0x55, 0x66);
    assert(transaction_count == 18);
    const uint32_t results[] = {0xffff0fff, 0xffff02ff, 0xffff021f, 0xffff0213, 0xf4ff0213, 0xf45f0213, 0xf4560213};
    for (unsigned i = 0; i < 7; ++i) expect(5 + 2 * i, 'W', 0x60035120, results[i]);
    clear_trace(); hal_clr_extern_pti(); assert(transaction_count == 8);
    expect(7, 'W', 0x60035120, 0xf4560000);
    clear_trace(); registers[register_index(0x60033c6c)] = UINT32_MAX;
    hal_enable_extern_coex(); assert(transaction_count == 4);
    expect(1, 'W', 0x60033c6c, 0xfbffffff); expect(3, 'W', 0x60035084, 1);
    clear_trace(); hal_disable_extern_coex(); expect(1, 'W', 0x60033c6c, UINT32_MAX); expect(3, 'W', 0x60035084, 0);
    clear_trace(); registers[register_index(0x600350a4)] = 0xabcdef00;
    hal_set_extern_coex_gpio(0xfe); expect(1, 'W', 0x600350a4, 0xabcdef06);
    clear_trace(); matrix_result = 7;
    assert(coex_gpio_debug_matrix_config(0) == 7 && !transaction_count);
    int before = matrix_calls;
    assert(coex_gpio_debug_matrix_config(2) == ESP_ERR_INVALID_ARG && matrix_calls == before);
    matrix_result = 0;
    assert(coex_gpio_debug_matrix_init() == 0 && matrix_calls == before + 2 && transaction_count == 4);
    expect(1, 'W', 0x600350a4, 0xabcdef07);
}
static void select_state(uint16_t w, uint16_t l, uint16_t t, uint16_t e, uint16_t i, coex_schedule *expected)
{
    coex_schm_env.wifi_status = w; coex_schm_env.ble_status = l; coex_schm_env.bt_status = t;
    coex_schm_env.external_status = e; coex_schm_env.i154_status = i;
    coex_schm_env.current = NULL;
    /* A high-only input marks changed without modifying any stored status. */
    coex_schm_status_bit_set(0, 0x10000);
    assert(coex_schm_env.current == expected && coex_schm_env.wifi_status == w);
}
static void test_selection(void)
{
#ifdef COEX_TEST_WEAK_OVERRIDE
    select_state(1, 0, 0, 0, 0, &coex_schm_bt_idle_wifi_scan);
    assert(coex_schm_env.current->period == 99 && coex_schm_env.current->phases[0].wifi_control == 7);
#endif
    select_state(0, 0xffff, 0xffff, 0, 1, &coex_schm_all_default);
    select_state(7, 0xffff, 0xffff, 3, 1, &coex_schm_external_coex_wifi_scan);
    select_state(6, 0xffff, 0xffff, 3, 1, &coex_schm_external_coex_wifi_connecting);
    select_state(4, 0, 0, 3, 0, &coex_schm_external_coex_wifi_default_rxonly);
    select_state(64, 0, 0, 1, 0, &coex_schm_external_coex_wifi_default);
    select_state(1, 0, 0x72, 0, 0, &coex_schm_bt_page_wifi_scan);
    select_state(2, 0, 0x72, 0, 0, &coex_schm_bt_a2dp_paused_wifi_connecting);
    select_state(4, 0, 0x72, 0, 0, &coex_schm_bt_page_wifi_conn);
    select_state(7, 0, 0x12, 0, 0, &coex_schm_bt_inq_wifi_scan);
    select_state(6, 0, 0x12, 0, 0, &coex_schm_bt_a2dp_wifi_connecting);
    select_state(1, 0x38, 0xff, 0, 0, &coex_schm_ble_mesh_config_bt_a2dp_paused_wifi_scan);
    select_state(2, 0x32, 0x1c, 0, 0, &coex_schm_ble_mesh_traffic_bt_a2dp_wifi_connecting);
    select_state(4, 0x802, 0x42, 0, 0, &coex_schm_ble_mesh_traffic_bt_default_wifi_conn);
    select_state(4, 0x22, 0, 0, 0, &coex_schm_ble_mesh_standby_wifi_conn);
    select_state(1, 0, 0, 0, 1, &coex_schm_ble_default_bt_idle_wifi_scan);
    select_state(4, 0x80, 0x12, 0, 0, &coex_schm_ble_default_bt_a2dp_wifi_conn);
    select_state(2, 0x80, 0x40, 0, 0, &coex_schm_ble_default_bt_default_wifi_connecting);
    coex_schedule *previous = coex_schm_env.current;
    coex_schm_status_bit_clear(0, 0x10000); assert(coex_schm_env.current == previous);
    coex_schm_status_bit_set(5, UINT32_MAX); assert(coex_schm_env.current == previous);
    take_result = 2; coex_schm_status_bit_set(0, 1); assert(coex_schm_env.wifi_status == 2); take_result = 1;
}
static void test_phases(void)
{
    coex_schm_env.wifi_status = 4; coex_schm_env.ble_status = coex_schm_env.bt_status = 0;
    coex_schm_env.external_status = coex_schm_env.i154_status = 0;
    coex_schm_env.current = &combined; coex_schm_env.phase_index = 1; coex_schm_env.interval = UINT32_MAX;
    coex_schm_env.wifi_callback = wifi; coex_schm_env.bt_callback = bt;
    coex_set_gpio_debug_cb(debug); callback_count = 0;
    int old_arms = arms, old_disarms = disarms;
    assert(coex_schm_process_restart() == 0 && coex_schm_env.phase_index == 0);
    assert(arms == old_arms + 1 && disarms == old_disarms + 1 && delay == (uint32_t)(UINT32_MAX * 13u * 25u));
    assert(callback_count == 3 && memcmp(callbacks, "DWB", 3) == 0);
    callback_count = 0; timeout_handler(NULL);
    assert(coex_schm_env.phase_index == 1 && arms == old_arms + 1 && callback_count == 2 && !memcmp(callbacks, "DB", 2));
    old_disarms = disarms; callback_count = 0; timeout_handler(NULL);
    assert(disarms == old_disarms && callback_count == 1 && callbacks[0] == 'D');
    coex_schm_env.wifi_status = 1; coex_schm_env.ble_status = 8;
    timeout_handler(NULL); assert(coex_schm_env.phase_index == 0 && disarms == old_disarms + 1);
    old_arms = arms; timeout_handler(NULL); assert(arms == old_arms + 1 && coex_schm_env.phase_index == 1);
    coex_schm_env.current = NULL; old_disarms = disarms;
    assert(coex_schm_process_restart() == -1 && disarms == old_disarms);
    take_result = -1; int old_debug = debug_calls;
    assert(coex_schm_process_restart() == -1 && debug_calls == old_debug); take_result = 1;
    coex_schm_env.current = &coex_schm_all_default; replace_on_arm = 1; callback_count = 0;
    assert(coex_schm_process_restart() == 0 && callback_count == 3 && !memcmp(callbacks, "DWB", 3));
    replace_on_arm = 0;
    /* Status restart is exactly false->true loop eligibility at special W. */
    coex_schm_env.wifi_status = 1; coex_schm_env.ble_status = coex_schm_env.bt_status = 0;
    coex_schm_env.external_status = coex_schm_env.i154_status = 0;
    old_disarms = disarms; coex_schm_status_bit_set(1, 8); assert(disarms == old_disarms + 1);
    old_disarms = disarms; coex_schm_status_bit_set(2, 4); assert(disarms == old_disarms);
    coex_schm_status_bit_clear(1, UINT32_MAX); assert(disarms == old_disarms);
    coex_schm_env.wifi_status = 0; coex_schm_env.ble_status = 8; coex_schm_env.bt_status = 0;
    coex_schm_status_bit_set(2, 4); assert(disarms == old_disarms); /* BLE+BT is one group */
}
static void test_external(void)
{
    coex_schm_env.wifi_status = coex_schm_env.ble_status = coex_schm_env.bt_status = 0;
    coex_schm_env.external_status = coex_schm_env.i154_status = 0;
    coex_env.semaphore = &semaphores[0]; clear_trace();
    assert(ic_set_extern_coex(-1, 1, 7) == 0);
    assert(coex_schm_env.external_status == 1 && registers[register_index(0x60035120)] == 0xf0000f33);
    assert(registers[register_index(0x60035084)] & 1);
    esp_coex_external_set_txline(true); assert(coex_schm_env.external_status == 3);
    clear_trace(); esp_coex_external_stop();
    assert(!coex_schm_env.external_status && !(registers[register_index(0x60035084)] & 1));
    esp_coex_external_set_wire_type((external_coex_wire_t)0xabcdef);
    assert((uint32_t)esp_coex_external_get_wire_type() == 0xabcdef);
}
int main(void)
{
    g_coa_funcs_p = &adapter;
    create_fail = 1; assert(coex_schm_init() == ESP_ERR_NO_MEM); create_fail = 0;
    assert(coex_schm_init() == 0 && setups == 1 && timeout_handler);
    int before = created; assert(coex_schm_init() == 0 && setups == 2 && created == before);
    test_mmio(); test_selection(); test_phases(); test_external();
    void *handle = coex_schm_env.semaphore; coex_schm_deinit();
    assert(done_calls == 1 && coex_schm_env.semaphore == handle && !held);
    puts("libcoexist scheduler/MMIO/external host tests: PASS (ASan/UBSan)");
    return 0;
}
