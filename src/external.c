/* SPDX-License-Identifier: MPL-2.0 */
#include "coexist_subset.h"
static uint32_t wire_type;
static uint32_t priority_for_level(int level) { return level == 1 ? 15 : level == 0 ? 11 : 3; }
int ic_set_extern_coex(int level1, int level2, int level3)
{
    (void)level1;
    (void)coex_core_enable();
    coex_schm_status_bit_set(3, 1);
    hal_set_extern_coex_gpio(7);
    hal_set_extern_pti(3, priority_for_level(level2), priority_for_level(level3), 0, 0, 0);
    hal_enable_extern_coex();
    return 0;
}
void ic_stop_extern_coex(void)
{
    coex_schm_status_bit_clear(3, 2);
    coex_schm_status_bit_clear(3, 1);
    (void)coex_core_disable();
    hal_clr_extern_pti();
    hal_disable_extern_coex();
}
esp_err_t esp_coex_external_set(esp_coex_pti_level_t level1, esp_coex_pti_level_t level2, esp_coex_pti_level_t level3)
{ return ic_set_extern_coex((int)level1, (int)level2, (int)level3); }
void esp_coex_external_stop(void) { ic_stop_extern_coex(); }
void esp_coex_external_set_txline(bool enabled)
{
    if (enabled) coex_schm_status_bit_set(3, 2);
    else coex_schm_status_bit_clear(3, 2);
}
void esp_coex_external_set_wire_type(external_coex_wire_t type) { wire_type = (uint32_t)type; }
int esp_coex_external_get_wire_type(void) { return (int)wire_type; }
