/* SPDX-License-Identifier: MPL-2.0 */
#ifndef LIBCOEXIST_SUBSET_H
#define LIBCOEXIST_SUBSET_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "sdkconfig.h"
#include "soc/soc_caps.h"
#include "esp_coexist_internal.h"
#include "esp_coexist_debug.h"
#ifdef COEX_TARGET
#include "rom/ets_sys.h"
#else
#include "ets_timer_type_excerpt.h"
#endif
/* Public headers conditionally hide these declarations with optional SDK
 * features; the pinned archive exports these interfaces in either case. */
#if !CONFIG_ESP_COEX_GPIO_DEBUG
typedef enum { COEX_GPIO_DEBUG_SIG_POSE, COEX_GPIO_DEBUG_SIG_NEGA } coex_gpio_debug_sig_t;
void coex_set_gpio_debug_cb(void (*callback)(int, coex_gpio_debug_sig_t));
int coex_gpio_debug_max_event_get(void);
esp_err_t coex_gpio_debug_matrix_init(void);
esp_err_t coex_gpio_debug_matrix_config(int event);
#endif
#if !CONFIG_EXTERNAL_COEX_ENABLE
typedef enum { EXTERN_COEX_PTI_MID = 0, EXTERN_COEX_PTI_HIGH, EXTERN_COEX_PTI_NUM } esp_coex_pti_level_t;
esp_err_t esp_coex_external_set(esp_coex_pti_level_t, esp_coex_pti_level_t, esp_coex_pti_level_t);
void esp_coex_external_stop(void);
void esp_coex_external_set_wire_type(external_coex_wire_t);
#endif
#if !CONFIG_EXTERNAL_COEX_ENABLE || !SOC_EXTERNAL_COEX_LEADER_TX_LINE
void esp_coex_external_set_txline(bool enabled);
#endif
#if !CONFIG_ESP_COEX_POWER_MANAGEMENT
int coex_schm_flexible_period_set(uint8_t period);
uint8_t coex_schm_flexible_period_get(void);
#endif
#if !SOC_COEX_HW_PTI
int coex_pti_get(uint32_t event, uint8_t *priority);
#endif

/* Internal signatures follow normalized ABI declarations in closure 05. */
typedef int (*coex_start_callback)(void);
typedef int (*coex_scheme_callback)(int);
typedef struct {
    uint8_t weight, wifi_control, bt_control_a, bt_control_b;
} coex_phase;
typedef struct {
    uint8_t phase_count, period;
    coex_phase phases[];
} coex_schedule;
typedef struct {
    void *semaphore;
    uint32_t status, reserved;
    coex_start_callback start_callback;
} coex_core_state;
typedef struct {
    coex_schedule *current;
    uint8_t phase_index, reserved5;
    uint16_t wifi_status, ble_status, bt_status;
    uint32_t interval;
    void *semaphore;
    coex_scheme_callback bt_callback;
    uint8_t primary, secondary, reserved26[2];
    coex_wifi_channel_change_cb_t channel_callback;
    ETSTimer timer;
    coex_scheme_callback wifi_callback;
    uint16_t external_status;
    uint8_t flexible_period, reserved59;
    uint16_t i154_status, reserved62;
} coex_scheduler_state;
extern coex_core_state coex_env;
extern coex_scheduler_state coex_schm_env;
/* ROM-owned cells are declared, never allocated by this archive. */
extern coex_adapter_funcs_t *g_coa_funcs_p;
extern coex_core_state *coex_env_ptr;
extern coex_scheduler_state *coex_schm_env_ptr;
extern uint8_t *coex_pti_tab_ptr;
extern uint32_t *g_coex_param_ptr;
/* Defaults and identities follow verified closures 01, 02 and 06. */
extern uint8_t coex_pti_tab[42];
extern uint32_t g_coex_param[5];
extern const char *g_coex_adapter_funcs_md5, *g_coex_i154_funcs_md5, *g_coex_basic_md5;
extern const char *libcoexist_reversion_git, *libcoexist_reversion_remote;
extern uint32_t coex_log_level;
int esp_coex_i154_funcs_md5_check(const char *md5);
int coex_core_pti_set(uint32_t event, uint8_t priority);
int coex_core_pti_get(uint32_t event, uint8_t *priority);
int coex_pti_set(uint32_t event, uint8_t priority);
int coex_core_event_duration_get(uint32_t event, uint32_t *duration);
int coex_core_pre_init(void);
int coex_core_init(void);
int coex_core_deinit(void);
int coex_core_enable(void);
int coex_core_disable(void);
uint32_t coex_core_status_get(void);
int coex_core_register_start_cb(coex_start_callback callback);
bool coex_is_running(void);
int coex_schm_lock(void);
int coex_schm_unlock(void);
uint32_t coex_schm_status_get(uint32_t type);
uint32_t coex_schm_status_bitmap_get(void);
void coex_dbg_output(int event, coex_gpio_debug_sig_t signal);
void coex_dbg_set_log_level(uint32_t level);
void coex_rom_data_init(void);
uint8_t coex_core_timer_idx_get(uint32_t event);
int coex_core_request(uint8_t radio, uint32_t event, uint32_t latency, uint32_t duration);
int coex_core_release(uint8_t radio, uint32_t event);
int coex_core_ble_conn_dyn_prio_get(bool *low, bool *high);
void coex_hw_timer_set(uint8_t index, uint8_t radio, uint8_t priority, uint32_t latency, uint32_t duration);
void coex_hw_timer_enable(uint8_t index);
void coex_hw_timer_disable(uint8_t index);
typedef struct {
    int (*pti_get)(uint32_t, uint8_t *);
    uint8_t (*timer_idx_get)(uint32_t);
    int (*request)(uint8_t, uint32_t, uint32_t, uint32_t);
    int (*release)(uint8_t, uint32_t);
    int (*ble_conn_dyn_prio_get)(bool *, bool *);
    int (*event_duration_get)(uint32_t, uint32_t *);
    uint32_t (*status_get)(void);
    void (*timer_set)(uint8_t, uint8_t, uint8_t, uint32_t, uint32_t);
    void (*timer_enable)(uint8_t);
    void (*timer_disable)(uint8_t);
    int (*schm_lock)(void);
    int (*schm_unlock)(void);
} coex_rom_dispatch;
extern coex_rom_dispatch *coexist_funcs;
void coex_rom_osi_funcs_deinit(void);
const char *esp_coex_rom_version_get(void);
void coex_schm_deinit(void);
void coex_hw_timer_force(uint8_t index);
void coex_hw_timer_unforce(uint8_t index);
int coex_hw_debug_matrix_config(int event);
int coex_hw_debug_matrix_init(void);
void hal_coex_set_wait_time(uint32_t first, uint32_t second);
void hal_enable_extern_coex(void);
void hal_disable_extern_coex(void);
void hal_set_extern_coex_gpio(uint32_t selection);
void hal_set_extern_pti(uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5);
void hal_clr_extern_pti(void);
int ic_set_extern_coex(int level1, int level2, int level3);
void ic_stop_extern_coex(void);
int esp_coex_external_get_wire_type(void);
int coex_schm_init(void);
int coex_rom_osi_funcs_init(void);
#endif
