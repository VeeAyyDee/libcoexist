/* SPDX-License-Identifier: MPL-2.0 */
#include "coexist_subset.h"
#include "coexist_diagnostics.h"
#include "coexist_policy.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* These explicit test doubles are not included in libcoexist.a. */
coex_adapter_funcs_t *g_coa_funcs_p;
coex_core_state *coex_env_ptr;
coex_scheduler_state *coex_schm_env_ptr;
uint8_t *coex_pti_tab_ptr;
uint32_t *g_coex_param_ptr;
coex_rom_dispatch *coexist_funcs;
static int semaphore, held, take_result = 1, give_result = 1;
static int creates, deletes, takes, gives, yields, isr, wake_request;
static int start_calls, channel_calls, scheme_calls, lpclk_calls, debug_calls;
static int scheduler_result, allocation_fail, allocations, frees, create_fail;
static uint32_t forwarded_type, forwarded_status, timer_latency, timer_duration;
static int forwards, timer_sets, timer_enables, timer_disables;
static uint8_t timer_index, timer_radio, timer_priority;
static void *create(uint32_t maximum, uint32_t initial)
{
    assert(maximum == 1 && initial == 1);
    ++creates;
    return create_fail ? NULL : &semaphore;
}
static void destroy(void *handle) { assert(handle == &semaphore); ++deletes; }
static int32_t take(void *handle, uint32_t ticks)
{
    assert(handle == &semaphore && ticks == UINT32_MAX);
    ++takes;
    if (take_result == 1) { assert(!held); held = 1; }
    return take_result;
}
static int32_t give(void *handle)
{
    assert(handle == &semaphore);
    ++gives;
    assert(held);
    held = 0;
    return give_result;
}
static int32_t take_isr(void *handle, void *wake)
{
    assert(*(int *)wake == 0);
    *(int *)wake = wake_request;
    return take(handle, UINT32_MAX);
}
static int32_t give_isr(void *handle, void *wake)
{
    assert(*(int *)wake == 0);
    *(int *)wake = wake_request;
    return give(handle);
}
static void yield(void) { ++yields; }
static int in_isr(void) { return isr; }
static void *allocate(size_t size)
{
    assert(size == sizeof(coex_rom_dispatch));
    ++allocations;
    return allocation_fail ? NULL : malloc(size);
}
static void release_memory(void *ptr)
{
    /* Original order retains the published pointer while free is called. */
    assert(ptr == coexist_funcs);
    ++frees;
    free(ptr);
}
const char *esp_coex_rom_version_get(void) { return "test-rom"; }
int coex_schm_init(void) { return scheduler_result; }
int coex_core_ble_conn_dyn_prio_get(bool *low, bool *high)
{ *low = false; *high = true; return 0; }
void coex_schm_status_bit_set(uint32_t type, uint32_t status)
{ ++forwards; forwarded_type = type; forwarded_status = status; }
void coex_schm_status_bit_clear(uint32_t type, uint32_t status)
{ ++forwards; forwarded_type = type; forwarded_status = status; }
void coex_hw_timer_set(uint8_t index, uint8_t radio, uint8_t priority, uint32_t latency, uint32_t duration)
{
    ++timer_sets; timer_index = index; timer_radio = radio;
    timer_priority = priority; timer_latency = latency; timer_duration = duration;
}
void coex_hw_timer_enable(uint8_t index) { assert(index == timer_index); ++timer_enables; }
void coex_hw_timer_disable(uint8_t index) { timer_index = index; ++timer_disables; }
static int start(void) { assert(!held); ++start_calls; return 97; }
static int scheme(int event) { assert(!held && event == 0); ++scheme_calls; return 89; }
static void channel(uint8_t primary, uint8_t secondary)
{
    assert(!held && primary == 11 && secondary == 2);
    ++channel_calls;
    uint8_t p, s;
    assert(coex_wifi_channel_get(&p, &s) == 0 && p == primary && s == secondary);
}
static int lpclk(void) { ++lpclk_calls; return 37; }
static void debug(int event, coex_gpio_debug_sig_t signal)
{ assert(event == 4 && signal == COEX_GPIO_DEBUG_SIG_NEGA); ++debug_calls; }
static int64_t get_time(void) { return INT64_C(0x100000001); }
int coexist_printf(const char *format, ...) { assert(format); return 0; }
static coex_adapter_funcs_t adapter = {
    ._esp_timer_get_time = get_time,
    ._version = COEX_ADAPTER_VERSION, ._magic = (int32_t)COEX_ADAPTER_MAGIC,
    ._task_yield_from_isr = yield, ._semphr_create = create, ._semphr_delete = destroy,
    ._semphr_take_from_isr = take_isr, ._semphr_give_from_isr = give_isr,
    ._semphr_take = take, ._semphr_give = give, ._is_in_isr = in_isr,
    ._malloc_internal = allocate, ._free = release_memory,
};
static void test_interface(void)
{
    assert(esp_coex_adapter_register(NULL) == ESP_ERR_INVALID_ARG);
    coex_adapter_funcs_t bad = adapter;
    bad._version = 1;
    assert(esp_coex_adapter_register(&bad) == ESP_ERR_INVALID_ARG);
    bad = adapter; bad._magic = 0;
    assert(esp_coex_adapter_register(&bad) == ESP_ERR_INVALID_ARG);
    assert(esp_coex_adapter_register(&adapter) == 0 && g_coa_funcs_p == &adapter);
    assert(esp_coex_adapter_register(NULL) == 0 && g_coa_funcs_p == &adapter);
    assert(esp_coex_adapter_register(&bad) == 0 && g_coa_funcs_p == &adapter);
    assert(esp_coex_adapter_funcs_md5_check(NULL) == 0);
    assert(esp_coex_adapter_funcs_md5_check("6964e7fsuffix") == 0);
    assert(esp_coex_adapter_funcs_md5_check("6964e7") == ESP_ERR_INVALID_ARG);
    assert(esp_coex_adapter_funcs_md5_check("bad") == ESP_ERR_INVALID_ARG);
    assert(esp_coex_i154_funcs_md5_check("85dae7a") == 0);
    assert(esp_coex_i154_funcs_md5_check("") == ESP_ERR_INVALID_ARG);
    assert(strcmp(g_coex_basic_md5, "7d9bf44") == 0);
    for (unsigned type = 0; type < 7; ++type) {
        for (uint32_t value = 0; value < 65; ++value) {
            bool valid = type == 1 ? value == 8 || value == 16 || value == 32 :
                         type == 2 ? value == 16 || value == 32 :
                         type == 4 ? value == 1 : type != 0;
            int before = forwards;
            int result = esp_coex_status_bit_set((esp_coex_status_type_t)type, value);
            assert(result == (valid ? 0 : ESP_ERR_INVALID_ARG));
            assert(forwards == before + (valid ? 1 : 0));
            if (valid) assert(forwarded_type == type && forwarded_status == value);
            result = esp_coex_status_bit_clear((esp_coex_status_type_t)type, value);
            assert(result == (valid ? 0 : ESP_ERR_INVALID_ARG));
        }
    }
    coex_version_t version = {0};
    assert(coex_version_get_value(NULL) == ESP_ERR_INVALID_ARG);
    assert(coex_version_get_value(&version) == 0);
    assert(version.major == 2 && version.minor == 0 && version.patch == 0);
    assert(strstr(coex_version_get(), "fresh"));
    coex_update_lpclk_interval();
    coex_wifi_register_update_lpclk_callback(lpclk);
    coex_update_lpclk_interval();
    coex_wifi_register_update_lpclk_callback(NULL);
    coex_update_lpclk_interval();
    assert(lpclk_calls == 1);
    coex_set_gpio_debug_cb(debug);
    coex_dbg_output(4, COEX_GPIO_DEBUG_SIG_NEGA);
    coex_set_gpio_debug_cb(NULL);
    coex_dbg_output(0, COEX_GPIO_DEBUG_SIG_POSE);
    assert(debug_calls == 1 && coex_gpio_debug_max_event_get() == 5);
    coex_dbg_set_log_level(0xdeadbeef);
    assert(coex_log_level == 0xdeadbeef);
}
static void test_lifecycle(void)
{
    assert(coex_core_enable() == -1);
    assert(coex_core_register_start_cb(start) == ESP_ERR_INVALID_STATE);
    create_fail = 1;
    assert(coex_core_pre_init() == ESP_ERR_NO_MEM && !coex_env.semaphore);
    create_fail = 0; scheduler_result = 7;
    assert(coex_core_pre_init() == ESP_ERR_NO_MEM && deletes == 1);
    assert(coex_env.semaphore == &semaphore);
    /* This failed-preinit handle is intentionally dangling by contract. The
     * test resets injected state; production must not retry it blindly. */
    coex_env.semaphore = NULL; scheduler_result = 0; take_result = 2;
    assert(coex_core_pre_init() == -1 && allocations == 0);
    take_result = 1; allocation_fail = 1;
    assert(coex_core_pre_init() == ESP_ERR_NO_MEM && !held && !coexist_funcs);
    allocation_fail = 0;
    assert(coex_core_pre_init() == 0 && !held);
    assert(coex_env_ptr == &coex_env && coex_schm_env_ptr == &coex_schm_env);
    assert(coex_pti_tab_ptr == coex_pti_tab && g_coex_param_ptr == g_coex_param);
    assert(coexist_funcs->pti_get == coex_core_pti_get);
    assert(coexist_funcs->timer_idx_get == coex_core_timer_idx_get);
    assert(coexist_funcs->request == coex_core_request);
    assert(coexist_funcs->release == coex_core_release);
    assert(coexist_funcs->ble_conn_dyn_prio_get == coex_core_ble_conn_dyn_prio_get);
    assert(coexist_funcs->event_duration_get == coex_core_event_duration_get);
    assert(coexist_funcs->status_get == coex_core_status_get);
    assert(coexist_funcs->timer_set == coex_hw_timer_set);
    assert(coexist_funcs->timer_enable == coex_hw_timer_enable);
    assert(coexist_funcs->timer_disable == coex_hw_timer_disable);
    assert(coexist_funcs->schm_lock == coex_schm_lock && coexist_funcs->schm_unlock == coex_schm_unlock);
    int before = allocations;
    coex_rom_dispatch *existing = coexist_funcs;
    assert(coex_core_pre_init() == 0 && coexist_funcs == existing && allocations == before);
    coex_rom_osi_funcs_deinit();
    assert(!coexist_funcs && frees == 1 && coex_env_ptr == &coex_env);
    coex_rom_osi_funcs_deinit();
    assert(frees == 1);
    /* One-time data guard persists across dispatch deinit/reinit. */
    coex_env_ptr = NULL;
    assert(coex_rom_osi_funcs_init() == 0 && !coex_env_ptr);
    coex_env_ptr = &coex_env;
    assert(coex_register_start_cb(start) == 0);
    assert(coex_enable() == 0 && !coex_is_running() && start_calls == 0);
    assert(coex_enable() == 0 && coex_is_running() && start_calls == 1);
    assert(coex_enable() == 0 && start_calls == 2);
    coex_disable(); assert(coex_is_running());
    coex_disable(); assert(!coex_is_running());
    /* count=1; advance through 255, 0, 1, verifying modulo-256 behavior. */
    for (int i = 0; i < 254; ++i) assert(coex_core_enable() == 0);
    before = start_calls;
    assert(coex_core_enable() == 0 && start_calls == before && coex_is_running());
    assert(coex_core_enable() == 0 && start_calls == before);
    assert(coex_core_enable() == 0 && start_calls == before + 1);
    take_result = 0;
    before = gives;
    assert(coex_core_disable() == -1 && gives == before);
    assert(coex_core_register_start_cb(NULL) == ESP_ERR_INVALID_STATE);
    take_result = 1;
    isr = 1; wake_request = 1;
    assert(coex_core_disable() == 0 && !coex_is_running() && yields == 2);
    take_result = -1;
    assert(coex_core_enable() == -1 && yields == 3);
    take_result = 1; wake_request = 0;
    assert(coex_core_enable() == 0 && yields == 3);
    isr = 0;
    assert(coex_core_init() == 0 && coex_core_deinit() == 0);
    coex_rom_osi_funcs_deinit();
    assert(creates == 3);
}
static void test_accessors(void)
{
    assert(coex_schm_lock() == ESP_ERR_INVALID_STATE);
    assert(coex_schm_unlock() == ESP_ERR_INVALID_STATE);
    assert(coex_schm_curr_period_get() == 1);
    assert(coex_schm_curr_phase_idx_get() == -1);
    coex_schm_env.semaphore = &semaphore;
    take_result = 2;
    assert(coex_schm_interval_set(77) == -1 && coex_schm_interval_get() == 0);
    assert(coex_schm_flexible_period_set(9) == -1);
    take_result = 1;
    assert(coex_schm_interval_set(UINT32_MAX) == 0 && coex_schm_interval_get() == UINT32_MAX);
    assert(coex_schm_flexible_period_set(255) == 0 && coex_schm_flexible_period_get() == 255);
    assert(coex_schm_curr_phase_get() == NULL);
    coex_schm_env.current = &coex_schm_all_default;
    assert(coex_schm_register_callback(COEX_SCHM_CALLBACK_TYPE_BT, (void *)scheme) == 0);
    assert(coex_schm_register_callback(COEX_SCHM_CALLBACK_TYPE_I154, NULL) == -1);
    assert(coex_schm_curr_phase_idx_set(1) == 0 && scheme_calls == 1);
    assert(coex_schm_curr_phase_get() == &coex_schm_all_default.phases[1]);
    assert(coex_schm_curr_phase_idx_set(2) == -1);
    coex_schm_env.wifi_status = 64;
    assert(coex_schm_curr_phase_idx_set(2) == 0 && coex_schm_curr_phase_idx_get() == 0);
    assert(coex_schm_get_phase_by_idx(2) == NULL);
    assert(coex_schm_get_phase_by_idx(-1) == (void *)((uintptr_t)&coex_schm_all_default - 2u));
    assert(coex_schm_curr_phase_idx_set(-1) == 0 && coex_schm_curr_phase_idx_get() == 255);
    assert(coex_schm_curr_phase_get() == NULL);
    assert(coex_schm_curr_period_get() == 1);
    assert(coex_register_wifi_channel_change_callback(channel) == 0);
    assert(coex_wifi_channel_set(11, 2) == 0 && coex_wifi_channel_set(11, 2) == 0);
    assert(channel_calls == 2 && coex_wifi_channel_get(NULL, NULL) == ESP_ERR_INVALID_ARG);
    coex_schm_env.ble_status = 16; coex_schm_env.bt_status = 32;
    coex_schm_env.external_status = 2; coex_schm_env.i154_status = 1;
    assert(coex_schm_status_get(0) == 64 && coex_schm_status_get(1) == 16);
    assert(coex_schm_status_get(2) == 32 && coex_schm_status_get(3) == 0);
    assert(coex_schm_status_bitmap_get() == 31);
    coex_env.status = 17;
    assert(coex_status_get(31) == 0 && coex_status_get(30) == 17 && !coex_is_running());
    isr = 1; int before = yields;
    assert(coex_schm_lock() == 0 && coex_schm_unlock() == 0 && yields == before);
    isr = 0; give_result = 2;
    assert(coex_schm_lock() == 0 && coex_schm_unlock() == -1);
    give_result = 1;
}
static void test_policy_and_requests(void)
{
    uint8_t value = 0;
    assert(coex_pti_get(0, &value) == 0 && value == 14);
    assert(coex_pti_set(42, 1) == ESP_ERR_INVALID_ARG);
    assert(coex_pti_set(1, 16) == ESP_ERR_INVALID_ARG);
    assert(coex_pti_set(1, (uint8_t)0x107) == 0 && coex_pti_get(1, &value) == 0 && value == 7);
    assert(coex_core_pti_get(0, NULL) == ESP_ERR_INVALID_ARG);
    uint32_t duration = 99;
    const uint32_t events[] = {4, 7, 9, 39, 40}, expected[] = {25000, 20000, 5000, 25000, 50000};
    for (unsigned i = 0; i < 5; ++i) assert(coex_event_duration_get(events[i], &duration) == 0 && duration == expected[i]);
    assert(coex_event_duration_get(3, &duration) == -1 && duration == 0);
    assert(coex_event_duration_get(4, NULL) == -1);
    for (uint32_t event = 0; event < 256; ++event) {
        uint8_t expected_index = event == 1 ? 0 : event == 2 ? 1 : event == 38 ? 2 : event == 39 || event == 40 ? 3 : 255;
        assert(coex_core_timer_idx_get(event) == expected_index);
    }
    assert(coex_core_timer_idx_get(UINT32_MAX) == 255);
    assert(coex_wifi_request(3, 1, 2) == ESP_ERR_INVALID_ARG && !timer_sets);
    assert(coex_wifi_request(1, 123, 456) == 0 && timer_sets == 1 && timer_enables == 1);
    assert(timer_index == 0 && timer_radio == 1 && timer_priority == 7 && timer_latency == 123 && timer_duration == 456);
    assert(coex_bt_request(40, 0, UINT32_MAX) == 0 && timer_radio == 0 && timer_index == 3);
    assert(coex_wifi_release(3) == ESP_ERR_INVALID_ARG && !timer_disables);
    assert(coex_bt_release(38) == 0 && timer_index == 2 && timer_disables == 1);
}
int main(void)
{
    assert(coex_env.reserved == 256 && coex_schm_env.flexible_period == 1 && coex_log_level == 1);
    test_interface(); test_lifecycle(); test_accessors(); test_policy_and_requests();
    assert(!held && takes > 0 && gives > 0);
    puts("libcoexist bounded host tests: PASS (ASan/UBSan)");
    return 0;
}
