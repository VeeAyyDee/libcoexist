/* SPDX-License-Identifier: MPL-2.0 */
#include "coexist_subset.h"
#include "coexist_diagnostics.h"
int coex_rom_osi_funcs_init(void)
{
    const char *version = esp_coex_rom_version_get();
    (void)coexist_printf("libcoexist ROM: %s\n", version);
    coex_rom_data_init();
    if (coexist_funcs) return 0;
    /* sizeof is 48 on S3, native pointer size on the host test model. */
    coex_rom_dispatch *dispatch = g_coa_funcs_p->_malloc_internal(sizeof(*dispatch));
    if (!dispatch) { COEX_DIAG("ROM dispatch allocation failed"); return ESP_ERR_NO_MEM; }
    /* Preserve observed publication order. Caller must quiesce ROM readers. */
    coexist_funcs = dispatch;
    dispatch->pti_get = coex_core_pti_get;
    dispatch->timer_idx_get = coex_core_timer_idx_get;
    dispatch->request = coex_core_request;
    dispatch->release = coex_core_release;
    dispatch->ble_conn_dyn_prio_get = coex_core_ble_conn_dyn_prio_get;
    dispatch->event_duration_get = coex_core_event_duration_get;
    dispatch->status_get = coex_core_status_get;
    dispatch->timer_set = coex_hw_timer_set;
    dispatch->timer_enable = coex_hw_timer_enable;
    dispatch->timer_disable = coex_hw_timer_disable;
    dispatch->schm_lock = coex_schm_lock;
    dispatch->schm_unlock = coex_schm_unlock;
    return 0;
}
void coex_rom_osi_funcs_deinit(void)
{
    if (!coexist_funcs) return;
    g_coa_funcs_p->_free(coexist_funcs);
    coexist_funcs = NULL;
}
