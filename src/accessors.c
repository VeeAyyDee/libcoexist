/* SPDX-License-Identifier: MPL-2.0 */
#include "coexist_subset.h"
#include "coexist_diagnostics.h"
#define IRAM __attribute__((section(".coexiram")))
coex_scheduler_state coex_schm_env __attribute__((section(".dram1"))) = { .flexible_period = 1 };
IRAM int coex_schm_lock(void)
{
    if (!coex_schm_env.semaphore) return ESP_ERR_INVALID_STATE;
    return g_coa_funcs_p->_semphr_take(coex_schm_env.semaphore,
            COEX_ADAPTER_FUNCS_TIME_BLOCKING) == 1 ? 0 : -1;
}
IRAM int coex_schm_unlock(void)
{
    if (!coex_schm_env.semaphore) return ESP_ERR_INVALID_STATE;
    return g_coa_funcs_p->_semphr_give(coex_schm_env.semaphore) == 1 ? 0 : -1;
}
int coex_schm_interval_set(uint32_t interval)
{
    if (coex_schm_lock()) return -1;
    coex_schm_env.interval = interval;
    (void)coex_schm_unlock();
    return 0;
}
uint32_t coex_schm_interval_get(void)
{
    if (coex_schm_lock()) return 0;
    uint32_t result = coex_schm_env.interval;
    (void)coex_schm_unlock();
    return result;
}
int coex_schm_flexible_period_set(uint8_t period)
{
    if (coex_schm_lock()) return -1;
    coex_schm_env.flexible_period = period;
    (void)coex_schm_unlock();
    return 0;
}
uint8_t coex_schm_flexible_period_get(void) { return coex_schm_env.flexible_period; }
int coex_schm_curr_phase_idx_get(void)
{
    if (coex_schm_lock()) return -1;
    int result = coex_schm_env.phase_index;
    (void)coex_schm_unlock();
    return result;
}
uint8_t coex_schm_curr_period_get(void)
{
    if (coex_schm_lock()) return 1;
    uint8_t result = coex_schm_env.current ? coex_schm_env.current->period : 1;
    (void)coex_schm_unlock();
    return result;
}
void *coex_schm_get_phase_by_idx(int index)
{
    if (coex_schm_lock()) return NULL;
    coex_schedule *schedule = coex_schm_env.current;
    void *result = NULL;
    if (schedule && index < schedule->phase_count) {
        /* Negative indices are outside the supported caller domain. Avoid C
         * pointer arithmetic outside the object while preserving address math. */
        result = (void *)((uintptr_t)schedule + 2u + (uintptr_t)((intptr_t)index * 4));
    }
    (void)coex_schm_unlock();
    return result;
}
void *coex_schm_curr_phase_get(void)
{
    if (coex_schm_lock()) return NULL;
    coex_schedule *schedule = coex_schm_env.current;
    void *result = NULL;
    if (schedule && coex_schm_env.phase_index < schedule->phase_count)
        result = &schedule->phases[coex_schm_env.phase_index];
    (void)coex_schm_unlock();
    return result;
}
int coex_schm_curr_phase_idx_set(int index)
{
    if (coex_schm_lock()) return -1;
    coex_schedule *schedule = coex_schm_env.current;
    int result = -1;
    if (schedule) {
        if (coex_schm_env.wifi_status == 64 && index == schedule->phase_count) index = 0;
        if (index < schedule->phase_count) {
            coex_schm_env.phase_index = (uint8_t)index;
            result = 0;
        }
    }
    (void)coex_schm_unlock();
    if (!result && coex_schm_env.bt_callback) (void)coex_schm_env.bt_callback(0);
    return result;
}
int coex_schm_register_callback(coex_schm_callback_type_t type, void *callback)
{
    if (coex_schm_lock()) return -1;
    int result = 0;
    /* SDK uses void * for this callback ABI; S3 and host test platform permit
     * the explicit conversion. Compatible callback type is specified by closure 02. */
    if (type == COEX_SCHM_CALLBACK_TYPE_WIFI)
        coex_schm_env.wifi_callback = (coex_scheme_callback)callback;
    else if (type == COEX_SCHM_CALLBACK_TYPE_BT)
        coex_schm_env.bt_callback = (coex_scheme_callback)callback;
    else result = -1;
    (void)coex_schm_unlock();
    return result;
}
int coex_register_wifi_channel_change_callback(coex_wifi_channel_change_cb_t callback)
{
    if (coex_schm_lock()) return -1;
    coex_schm_env.channel_callback = callback;
    (void)coex_schm_unlock();
    return 0;
}
int coex_wifi_channel_set(uint8_t primary, uint8_t secondary)
{
    if (coex_schm_lock()) return -1;
    coex_schm_env.primary = primary;
    coex_schm_env.secondary = secondary;
    coex_wifi_channel_change_cb_t callback = coex_schm_env.channel_callback;
    (void)coex_schm_unlock();
    if (callback) callback(primary, secondary);
    return 0;
}
int coex_wifi_channel_get(uint8_t *primary, uint8_t *secondary)
{
    if (!primary || !secondary) { COEX_DIAG("channel output pointers invalid: %p %p", (void *)primary, (void *)secondary); return ESP_ERR_INVALID_ARG; }
    if (coex_schm_lock()) return -1;
    *primary = coex_schm_env.primary;
    *secondary = coex_schm_env.secondary;
    (void)coex_schm_unlock();
    return 0;
}
uint32_t coex_schm_status_get(uint32_t type)
{
    if (coex_schm_lock()) return 0;
    uint32_t status = 0;
    if (type == 0) status = coex_schm_env.wifi_status;
    if (type == 1) status = coex_schm_env.ble_status;
    if (type == 2) status = coex_schm_env.bt_status;
    (void)coex_schm_unlock();
    return status;
}
IRAM uint32_t coex_schm_status_bitmap_get(void)
{
    return (coex_schm_env.wifi_status ? 1u : 0u)
         | (coex_schm_env.ble_status ? 2u : 0u)
         | (coex_schm_env.bt_status ? 4u : 0u)
         | (coex_schm_env.external_status ? 8u : 0u)
         | (coex_schm_env.i154_status ? 16u : 0u);
}
