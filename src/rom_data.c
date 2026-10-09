/* SPDX-License-Identifier: MPL-2.0 */
#include "coexist_subset.h"
static uint8_t data_published;
void coex_rom_data_init(void)
{
    if (data_published) return;
    coex_env_ptr = &coex_env;
    coex_schm_env_ptr = &coex_schm_env;
    coex_pti_tab_ptr = coex_pti_tab;
    g_coex_param_ptr = g_coex_param;
    data_published = 1;
}
