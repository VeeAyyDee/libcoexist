/* SPDX-License-Identifier: MPL-2.0 */
#include "coexist_policy.h"
/* Link-time replacement must override both direct reads and selector tables. */
coex_schedule coex_schm_bt_idle_wifi_scan = {
    .phase_count = 1, .period = 99,
    .phases = {{ .weight = 100, .wifi_control = 7 }}
};
