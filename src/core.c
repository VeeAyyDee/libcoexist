/* SPDX-License-Identifier: MPL-2.0 */
#include "coexist_subset.h"
#include "coexist_diagnostics.h"
#define IRAM __attribute__((section(".coexiram")))
#define DRAM __attribute__((section(".dram1")))
coex_core_state coex_env DRAM = { .reserved = 256 };
static uint8_t enable_count DRAM;

static IRAM int core_lock(void)
{
    if (!coex_env.semaphore) return ESP_ERR_INVALID_STATE;
    int result;
    if (g_coa_funcs_p->_is_in_isr()) {
        int wake = 0;
        result = g_coa_funcs_p->_semphr_take_from_isr(coex_env.semaphore, &wake);
        if (wake) g_coa_funcs_p->_task_yield_from_isr();
    } else {
        result = g_coa_funcs_p->_semphr_take(coex_env.semaphore,
                                           COEX_ADAPTER_FUNCS_TIME_BLOCKING);
    }
    return result == 1 ? 0 : -1;
}
static IRAM void core_unlock(void)
{
    if (!coex_env.semaphore) return;
    if (g_coa_funcs_p->_is_in_isr()) {
        int wake = 0;
        (void)g_coa_funcs_p->_semphr_give_from_isr(coex_env.semaphore, &wake);
        if (wake) g_coa_funcs_p->_task_yield_from_isr();
    } else {
        (void)g_coa_funcs_p->_semphr_give(coex_env.semaphore);
    }
}
int coex_core_pre_init(void)
{
    if (!coex_env.semaphore) {
        coex_env.semaphore = g_coa_funcs_p->_semphr_create(1, 1);
        if (!coex_env.semaphore) { COEX_DIAG("core semaphore allocation failed"); return ESP_ERR_NO_MEM; }
    }
    if (coex_schm_init()) {
        COEX_DIAG("scheduler initialization failed");
        g_coa_funcs_p->_semphr_delete(coex_env.semaphore);
        /* Preserve observed storage; retry after this failure is unsafe. */
        return ESP_ERR_NO_MEM;
    }
    if (core_lock()) return -1;
    int result = coex_rom_osi_funcs_init();
    core_unlock();
    return result;
}
int coex_core_init(void) { return 0; }
int coex_core_deinit(void) { return 0; }
IRAM int coex_core_enable(void)
{
    if (core_lock()) return -1;
    enable_count = (uint8_t)(enable_count + 1u);
    bool notify = enable_count >= 2;
    if (notify) coex_env.status = 1;
    core_unlock();
    if (notify && coex_env.start_callback) (void)coex_env.start_callback();
    return 0;
}
IRAM int coex_core_disable(void)
{
    if (core_lock()) return -1;
    enable_count = (uint8_t)(enable_count - 1u);
    if (enable_count == 1) coex_env.status = 0;
    core_unlock();
    return 0;
}
IRAM uint32_t coex_core_status_get(void) { return coex_env.status; }
int coex_core_register_start_cb(coex_start_callback callback)
{
    if (core_lock()) return ESP_ERR_INVALID_STATE;
    coex_env.start_callback = callback;
    core_unlock();
    return 0;
}
IRAM int coex_core_pti_set(uint32_t event, uint8_t priority)
{
    if (event >= 42 || priority >= 16) return ESP_ERR_INVALID_ARG;
    coex_pti_tab[event] = priority;
    return 0;
}
IRAM int coex_core_pti_get(uint32_t event, uint8_t *priority)
{
    if (!priority) return ESP_ERR_INVALID_ARG;
    /* Valid-domain precondition: event < 42, matching the observed getter. */
    *priority = coex_pti_tab[event];
    return 0;
}
IRAM int coex_core_event_duration_get(uint32_t event, uint32_t *duration)
{
    if (!duration) return -1;
    switch (event) {
    case 4: *duration = g_coex_param[0]; break;
    case 7: *duration = g_coex_param[1]; break;
    case 9: *duration = g_coex_param[2]; break;
    case 39: *duration = g_coex_param[3]; break;
    case 40: *duration = g_coex_param[4]; break;
    default: *duration = 0; return -1;
    }
    return 0;
}
IRAM uint8_t coex_core_timer_idx_get(uint32_t event)
{
    switch (event) {
    case 1: return 0;
    case 2: return 1;
    case 38: return 2;
    case 39: case 40: return 3;
    default: return 255;
    }
}
IRAM int coex_core_request(uint8_t radio, uint32_t event, uint32_t latency, uint32_t duration)
{
    uint8_t index = coex_core_timer_idx_get(event);
    if (index == 255) return ESP_ERR_INVALID_ARG;
    coex_hw_timer_set(index, radio, coex_pti_tab[event], latency, duration);
    coex_hw_timer_enable(index);
    return 0;
}
IRAM int coex_core_release(uint8_t radio, uint32_t event)
{
    (void)radio;
    uint8_t index = coex_core_timer_idx_get(event);
    if (index == 255) return ESP_ERR_INVALID_ARG;
    coex_hw_timer_disable(index);
    return 0;
}
