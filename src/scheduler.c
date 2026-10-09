/* SPDX-License-Identifier: MPL-2.0 */
#include "coexist_subset.h"
#include "coexist_policy.h"
#include "coexist_diagnostics.h"
#define DRAM __attribute__((section(".dram1.tables")))
/* Each row is one behavioral category; columns are the three Wi-Fi states.
 * Named addresses preserve weak overrides and descriptor pointer identity. */
#define WIFI_ROW(prefix) { &prefix##_wifi_scan, &prefix##_wifi_connecting, &prefix##_wifi_conn }
enum bt_category { BT_IDLE, BT_PAGE, BT_INQUIRY, BT_PAUSED, BT_A2DP, BT_SNIFF_SCO, BT_CONNECTED, BT_PISCAN, BT_DEFAULT };
static coex_schedule *classic[9][3] DRAM = {
    WIFI_ROW(coex_schm_bt_idle), WIFI_ROW(coex_schm_bt_page), WIFI_ROW(coex_schm_bt_inq),
    WIFI_ROW(coex_schm_bt_a2dp_paused), WIFI_ROW(coex_schm_bt_a2dp), WIFI_ROW(coex_schm_bt_sniff_sco),
    WIFI_ROW(coex_schm_bt_conn), WIFI_ROW(coex_schm_bt_piscan), WIFI_ROW(coex_schm_bt_default)
};
#define MESH_ROWS(family) { WIFI_ROW(coex_schm_ble_mesh_##family), \
    WIFI_ROW(coex_schm_ble_mesh_##family##_bt_a2dp_paused), \
    WIFI_ROW(coex_schm_ble_mesh_##family##_bt_a2dp), \
    WIFI_ROW(coex_schm_ble_mesh_##family##_bt_sniff_sco), \
    WIFI_ROW(coex_schm_ble_mesh_##family##_bt_conn), \
    WIFI_ROW(coex_schm_ble_mesh_##family##_bt_piscan), \
    WIFI_ROW(coex_schm_ble_mesh_##family##_bt_default) }
static coex_schedule *mesh[3][7][3] DRAM = { MESH_ROWS(config), MESH_ROWS(traffic), MESH_ROWS(standby) };
static coex_schedule *ble_default[3][3] DRAM = {
    WIFI_ROW(coex_schm_ble_default_bt_idle), WIFI_ROW(coex_schm_ble_default_bt_a2dp), WIFI_ROW(coex_schm_ble_default_bt_default)
};
static bool loop_allowed(void)
{
    unsigned groups = (coex_schm_env.wifi_status != 0) +
        (unsigned)((coex_schm_env.ble_status | coex_schm_env.bt_status) != 0) +
        (unsigned)(coex_schm_env.external_status != 0) + (unsigned)(coex_schm_env.i154_status != 0);
    return groups >= 2;
}
static unsigned classic_category(uint16_t bt, unsigned wifi)
{
    if (!bt) return BT_IDLE;
    if (wifi == 1) {
        if (bt & 0x20) return BT_PAUSED;
        if (bt & 0x10) return BT_A2DP;
    }
    if (bt & 0x40) return BT_PAGE;
    if (bt & 0x02) return BT_INQUIRY;
    if (bt & 0x20) return BT_PAUSED;
    if (bt & 0x10) return BT_A2DP;
    if (bt & 0x88) return BT_SNIFF_SCO;
    if (bt & 0x04) return BT_CONNECTED;
    if (bt & 0x01) return BT_PISCAN;
    return BT_DEFAULT;
}
static unsigned mesh_category(uint16_t bt)
{
    if (!bt) return 0;
    if (bt & 0x20) return 1;
    if (bt & 0x10) return 2;
    if (bt & 0x88) return 3;
    if (bt & 0x04) return 4;
    if (bt & 0x01) return 5;
    return 6;
}
static void select_schedule(void)
{
    uint16_t wifi = coex_schm_env.wifi_status, ble = coex_schm_env.ble_status;
    uint16_t bt = coex_schm_env.bt_status, external = coex_schm_env.external_status;
    coex_schm_env.current = &coex_schm_all_default;
    if (external & 1) {
        coex_schm_env.current = wifi & 1 ? &coex_schm_external_coex_wifi_scan :
            wifi & 2 ? &coex_schm_external_coex_wifi_connecting :
            external & 2 ? &coex_schm_external_coex_wifi_default_rxonly : &coex_schm_external_coex_wifi_default;
        return;
    }
    unsigned suffix;
    if (wifi & 1) suffix = 0;
    else if (wifi & 2) suffix = 1;
    else if (wifi & 4) suffix = 2;
    else return;
    if (!ble && !coex_schm_env.i154_status) {
        coex_schm_env.current = classic[classic_category(bt, suffix)][suffix];
    } else if ((ble & 8) || (ble & 16) || (ble & 0x3a) == 2 || (ble & 32)) {
        unsigned family = ble & 8 ? 0 : (ble & 16) || (ble & 0x3a) == 2 ? 1 : 2;
        coex_schm_env.current = mesh[family][mesh_category(bt)][suffix];
    } else {
        unsigned category = !bt ? 0 : bt & 16 ? 1 : 2;
        coex_schm_env.current = ble_default[category][suffix];
    }
}
static bool special_wifi(void)
{
    uint16_t wifi = coex_schm_env.wifi_status;
    return wifi == 1 || wifi == 2 || wifi == 64;
}
static int change_phase(bool restart)
{
    if (coex_schm_lock()) return -1;
    coex_schedule *schedule = coex_schm_env.current;
    if (!schedule) goto failed;
    if (!restart && coex_schm_env.phase_index >= (int)schedule->phase_count - 1) {
        if (!special_wifi() || !loop_allowed()) goto failed;
        restart = true;
    }
    g_coa_funcs_p->_timer_disarm(&coex_schm_env.timer);
    coex_schm_env.phase_index = restart ? 0 : (uint8_t)(coex_schm_env.phase_index + 1u);
    if (coex_schm_env.phase_index != (int)schedule->phase_count - 1 || (special_wifi() && loop_allowed())) {
        uint32_t delay = (uint32_t)schedule->period * coex_schm_env.interval * schedule->phases[coex_schm_env.phase_index].weight;
        g_coa_funcs_p->_timer_arm_us(&coex_schm_env.timer, delay, false);
    }
    schedule = coex_schm_env.current;
    coex_phase *phase = &schedule->phases[coex_schm_env.phase_index];
    coex_scheme_callback wifi = phase->wifi_control ? coex_schm_env.wifi_callback : NULL;
    coex_scheme_callback bt = phase->bt_control_a || phase->bt_control_b ? coex_schm_env.bt_callback : NULL;
    (void)coex_schm_unlock();
    coex_dbg_output(4, (coex_gpio_debug_sig_t)2);
    if (wifi) (void)wifi(0);
    if (bt) (void)bt(0);
    return 0;
failed:
    (void)coex_schm_unlock();
    coex_dbg_output(4, (coex_gpio_debug_sig_t)2);
    return -1;
}
static uint16_t *status_field(uint32_t type)
{
    switch (type) {
    case 0: return &coex_schm_env.wifi_status;
    case 1: return &coex_schm_env.ble_status;
    case 2: return &coex_schm_env.bt_status;
    case 3: return &coex_schm_env.external_status;
    case 4: return &coex_schm_env.i154_status;
    default: return NULL;
    }
}
void coex_schm_status_bit_set(uint32_t type, uint32_t status)
{
    if (coex_schm_lock()) return;
    bool old_loop = loop_allowed();
    uint16_t *field = status_field(type);
    bool changed = field && (status & ~(uint32_t)*field);
    if (changed) { *field = (uint16_t)(*field | status); select_schedule(); }
    bool new_loop = loop_allowed();
    (void)coex_schm_unlock();
    if (changed && !old_loop && new_loop && special_wifi()) (void)change_phase(true);
}
void coex_schm_status_bit_clear(uint32_t type, uint32_t status)
{
    if (coex_schm_lock()) return;
    uint16_t *field = status_field(type);
    if (field && (status & *field)) {
        *field = (uint16_t)(*field & ~status);
        select_schedule();
    }
    (void)coex_schm_unlock();
}
int coex_schm_process_restart(void) { return change_phase(true); }
static void timeout(void *argument) { (void)argument; (void)change_phase(false); }
int coex_schm_init(void)
{
    if (!coex_schm_env.semaphore) {
        coex_schm_env.semaphore = g_coa_funcs_p->_semphr_create(1, 1);
        if (!coex_schm_env.semaphore) { COEX_DIAG("scheduler semaphore allocation failed"); return ESP_ERR_NO_MEM; }
    }
    coex_schm_env.current = &coex_schm_all_default;
    g_coa_funcs_p->_timer_setfn(&coex_schm_env.timer, (void *)timeout, NULL);
    return 0;
}
void coex_schm_deinit(void) { g_coa_funcs_p->_timer_done(&coex_schm_env.timer); }
