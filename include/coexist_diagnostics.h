/* SPDX-License-Identifier: MPL-2.0 */
#pragma once
#include "coexist_subset.h"
int coexist_printf(const char *format, ...);
/* Operational failures retain the adapter's 64-bit clock callback ABI. */
#define COEX_DIAG(format, ...) \
    ((void)coexist_printf("libcoexist [%lld]: " format "\n", \
        (long long)g_coa_funcs_p->_esp_timer_get_time(), ##__VA_ARGS__))
