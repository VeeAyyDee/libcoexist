/* SPDX-License-Identifier: MPL-2.0 */
#include "coexist_subset.h"
#include <string.h>
#include <stdio.h>
#include "coexist_diagnostics.h"
const char *libcoexist_reversion_git = "fresh-libcoexist-mpl2";
const char *libcoexist_reversion_remote = "original-source/MPL-2.0";
const char *g_coex_adapter_funcs_md5 = "6964e7f";
const char *g_coex_i154_funcs_md5 = "85dae7a";
const char *g_coex_basic_md5 = "7d9bf44";
static coex_set_lpclk_source_callback_t lpclk_callback;
static void (*debug_callback)(int, coex_gpio_debug_sig_t);
uint32_t coex_log_level __attribute__((section(".dram1"))) = 1;
static int check_identity(const char *provided, const char *expected)
{
    if (!provided) return 0;
    if (!strncmp(provided, expected, strlen(expected))) return 0;
    (void)coexist_printf("libcoexist interface mismatch: expected %s, supplied %s\n", expected, provided);
    return ESP_ERR_INVALID_ARG;
}
int esp_coex_adapter_funcs_md5_check(const char *md5)
{
    return check_identity(md5, g_coex_adapter_funcs_md5);
}
int esp_coex_i154_funcs_md5_check(const char *md5)
{
    return check_identity(md5, g_coex_i154_funcs_md5);
}
esp_err_t esp_coex_adapter_register(coex_adapter_funcs_t *adapter)
{
    if (g_coa_funcs_p) return 0;
    if (!adapter) { (void)puts("libcoexist: missing adapter table"); return ESP_ERR_INVALID_ARG; }
    if (adapter->_version != COEX_ADAPTER_VERSION || (uint32_t)adapter->_magic != COEX_ADAPTER_MAGIC) {
        (void)coexist_printf("libcoexist adapter identity: expected %u/%08x, supplied %u/%08x\n",
            (unsigned)COEX_ADAPTER_VERSION, (unsigned)COEX_ADAPTER_MAGIC,
            (unsigned)adapter->_version, (unsigned)adapter->_magic);
        return ESP_ERR_INVALID_ARG;
    }
    g_coa_funcs_p = adapter;
    return 0;
}
static bool valid_status(esp_coex_status_type_t type, uint32_t status)
{
    switch ((unsigned)type) {
    case 0: return false;
    case 1: return status == 8 || status == 16 || status == 32;
    case 2: return status == 16 || status == 32;
    case 4: return status == 1;
    default: return true;
    }
}
esp_err_t esp_coex_status_bit_set(esp_coex_status_type_t type, uint32_t status)
{
    if (!valid_status(type, status)) { COEX_DIAG("invalid status type=%u value=%u", (unsigned)type, (unsigned)status); return ESP_ERR_INVALID_ARG; }
    coex_schm_status_bit_set((uint32_t)type, status);
    return 0;
}
esp_err_t esp_coex_status_bit_clear(esp_coex_status_type_t type, uint32_t status)
{
    if (!valid_status(type, status)) { COEX_DIAG("invalid status type=%u value=%u", (unsigned)type, (unsigned)status); return ESP_ERR_INVALID_ARG; }
    coex_schm_status_bit_clear((uint32_t)type, status);
    return 0;
}
int coex_event_duration_get(uint32_t event, uint32_t *duration)
{ return coex_core_event_duration_get(event, duration); }
int coex_pti_get(uint32_t event, uint8_t *priority)
{ return coex_core_pti_get(event, priority); }
int coex_pti_set(uint32_t event, uint8_t priority)
{ return coex_core_pti_set(event, (uint8_t)priority); }
esp_err_t coex_pre_init(void) { (void)coexist_printf("libcoexist build %s (%s)\n", libcoexist_reversion_git, libcoexist_reversion_remote); return coex_core_pre_init(); }
esp_err_t coex_init(void) { return coex_core_init(); }
void coex_deinit(void) { (void)coex_core_deinit(); }
esp_err_t coex_enable(void) { return coex_core_enable(); }
void coex_disable(void) { (void)coex_core_disable(); }
bool coex_is_running(void) { return coex_core_status_get() == 1; }
uint32_t coex_status_get(uint8_t bitmap)
{
    return (coex_schm_status_bitmap_get() & ~(uint32_t)bitmap) ? coex_core_status_get() : 0;
}
esp_err_t coex_preference_set(coex_prefer_t preference) { (void)preference; return 0; }
const char *coex_version_get(void) { return "fresh libcoexist MPL-2.0, ABI 2.0.0"; }
esp_err_t coex_version_get_value(coex_version_t *version)
{
    if (!version) return ESP_ERR_INVALID_ARG;
    version->major = 2;
    version->minor = version->patch = 0;
    return 0;
}
void coex_wifi_register_update_lpclk_callback(coex_set_lpclk_source_callback_t callback)
{ lpclk_callback = callback; }
void coex_update_lpclk_interval(void) { if (lpclk_callback) (void)lpclk_callback(); }
int coex_register_start_cb(int (*callback)(void)) { return coex_core_register_start_cb(callback); }
void coex_set_gpio_debug_cb(void (*callback)(int, coex_gpio_debug_sig_t)) { debug_callback = callback; }
void coex_dbg_output(int event, coex_gpio_debug_sig_t signal)
{ if (debug_callback) debug_callback(event, signal); }
int coex_gpio_debug_max_event_get(void) { return 5; }
void coex_dbg_set_log_level(uint32_t level) { coex_log_level = level; }

int coex_wifi_request(uint32_t event, uint32_t latency, uint32_t duration)
{ return coex_core_request(1, event, latency, duration); }
int coex_bt_request(uint32_t event, uint32_t latency, uint32_t duration)
{ return coex_core_request(0, event, latency, duration); }
int coex_wifi_release(uint32_t event) { return coex_core_release(1, event); }
int coex_bt_release(uint32_t event) { return coex_core_release(0, event); }
