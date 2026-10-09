/* SPDX-License-Identifier: MPL-2.0 */
/* Generated from named facts by tools/generate_policy.py. */
#include "coexist_subset.h"
coex_schedule coex_schm_all_default __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 40, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 60, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_external_coex_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 12,
    .phases = {
        { .weight = 100, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_external_coex_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 10,
    .phases = {
        { .weight = 100, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_external_coex_wifi_default_rxonly __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 50, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 4, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_external_coex_wifi_default __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 50, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_idle_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 12,
    .phases = {
        { .weight = 100, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_page_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 40,
    .phases = {
        { .weight = 30, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 70, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_inq_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 30,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 60, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_a2dp_paused_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 17,
    .phases = {
        { .weight = 70, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 4 },
    }
};
coex_schedule coex_schm_bt_a2dp_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 20,
    .phases = {
        { .weight = 50, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_sniff_sco_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 12,
    .phases = {
        { .weight = 100, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_conn_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 17,
    .phases = {
        { .weight = 70, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_piscan_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 12,
    .phases = {
        { .weight = 100, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_default_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 24,
    .phases = {
        { .weight = 50, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_idle_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 10,
    .phases = {
        { .weight = 100, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_a2dp_paused_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 10,
    .phases = {
        { .weight = 70, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 4 },
    }
};
coex_schedule coex_schm_bt_a2dp_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 25,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 60, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_page_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 20,
    .phases = {
        { .weight = 50, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_inq_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 20,
    .phases = {
        { .weight = 50, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_sniff_sco_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 10,
    .phases = {
        { .weight = 100, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_conn_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 10,
    .phases = {
        { .weight = 70, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_piscan_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 10,
    .phases = {
        { .weight = 100, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_default_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 20,
    .phases = {
        { .weight = 50, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_idle_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 1,
    .phases = {
        { .weight = 100, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_page_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 30, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 70, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_inq_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 60, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_a2dp_paused_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 70, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 4 },
    }
};
coex_schedule coex_schm_bt_a2dp_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 40, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 60, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 4 },
    }
};
coex_schedule coex_schm_bt_sniff_sco_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 1,
    .phases = {
        { .weight = 100, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_conn_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 70, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 4 },
    }
};
coex_schedule coex_schm_bt_piscan_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 1,
    .phases = {
        { .weight = 100, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_bt_default_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 50, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_config_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 12,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 60, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_a2dp_paused_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 12,
    .phases = {
        { .weight = 25, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_a2dp_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 12,
    .phases = {
        { .weight = 25, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_sniff_sco_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 12,
    .phases = {
        { .weight = 45, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 0 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_conn_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 12,
    .phases = {
        { .weight = 25, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_piscan_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 12,
    .phases = {
        { .weight = 45, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 2 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 32 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_default_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 12,
    .phases = {
        { .weight = 25, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_config_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 10,
    .phases = {
        { .weight = 45, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_a2dp_paused_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 10,
    .phases = {
        { .weight = 25, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_a2dp_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 10,
    .phases = {
        { .weight = 25, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_sniff_sco_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 10,
    .phases = {
        { .weight = 45, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 0 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_conn_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 10,
    .phases = {
        { .weight = 25, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_piscan_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 10,
    .phases = {
        { .weight = 45, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 2 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 32 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_default_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 10,
    .phases = {
        { .weight = 25, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_config_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 45, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_a2dp_paused_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 1,
    .phases = {
        { .weight = 25, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_a2dp_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 1,
    .phases = {
        { .weight = 25, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_sniff_sco_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 45, .wifi_control = 3, .bt_control_a = 32, .bt_control_b = 0 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_conn_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 1,
    .phases = {
        { .weight = 25, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_piscan_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 45, .wifi_control = 3, .bt_control_a = 32, .bt_control_b = 2 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 32 },
    }
};
coex_schedule coex_schm_ble_mesh_config_bt_default_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 1,
    .phases = {
        { .weight = 25, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 55, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 24,
    .phases = {
        { .weight = 50, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_a2dp_paused_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 30,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_a2dp_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 5, .period = 34,
    .phases = {
        { .weight = 30, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_sniff_sco_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 24,
    .phases = {
        { .weight = 50, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_conn_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 30,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_piscan_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 20,
    .phases = {
        { .weight = 60, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 2 },
        { .weight = 40, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 32 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_default_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 5, .period = 30,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 10,
    .phases = {
        { .weight = 70, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_a2dp_paused_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 25,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_a2dp_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 5, .period = 30,
    .phases = {
        { .weight = 30, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_sniff_sco_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 10,
    .phases = {
        { .weight = 70, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_conn_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 25,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_piscan_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 10,
    .phases = {
        { .weight = 70, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 2 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 32 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_default_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 5, .period = 30,
    .phases = {
        { .weight = 34, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 23, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
        { .weight = 23, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 60, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 40, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_a2dp_paused_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 1,
    .phases = {
        { .weight = 40, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_a2dp_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 1,
    .phases = {
        { .weight = 20, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_sniff_sco_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 70, .wifi_control = 3, .bt_control_a = 32, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_conn_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 1,
    .phases = {
        { .weight = 40, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_piscan_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 70, .wifi_control = 3, .bt_control_a = 32, .bt_control_b = 2 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 32 },
    }
};
coex_schedule coex_schm_ble_mesh_traffic_bt_default_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 1,
    .phases = {
        { .weight = 40, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 20,
    .phases = {
        { .weight = 60, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 40, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_a2dp_paused_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 30,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_a2dp_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 5, .period = 25,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 5, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 5, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_sniff_sco_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 20,
    .phases = {
        { .weight = 60, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 0 },
        { .weight = 40, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_conn_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 30,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_piscan_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 17,
    .phases = {
        { .weight = 70, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 2 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 32 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_default_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 5, .period = 30,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 5, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 5, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 10,
    .phases = {
        { .weight = 70, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_a2dp_paused_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 20,
    .phases = {
        { .weight = 45, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_a2dp_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 5, .period = 30,
    .phases = {
        { .weight = 30, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_sniff_sco_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 10,
    .phases = {
        { .weight = 70, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_conn_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 20,
    .phases = {
        { .weight = 45, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_piscan_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 10,
    .phases = {
        { .weight = 70, .wifi_control = 2, .bt_control_a = 32, .bt_control_b = 2 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 32 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_default_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 5, .period = 30,
    .phases = {
        { .weight = 34, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 23, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
        { .weight = 23, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 10, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 2,
    .phases = {
        { .weight = 80, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 20, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_a2dp_paused_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 2,
    .phases = {
        { .weight = 58, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 30, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 12, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_a2dp_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 2,
    .phases = {
        { .weight = 28, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 60, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 12, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_sniff_sco_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 2,
    .phases = {
        { .weight = 88, .wifi_control = 3, .bt_control_a = 32, .bt_control_b = 0 },
        { .weight = 12, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_conn_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 2,
    .phases = {
        { .weight = 63, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 25, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 12, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_piscan_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 2,
    .phases = {
        { .weight = 88, .wifi_control = 3, .bt_control_a = 32, .bt_control_b = 32 },
        { .weight = 12, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 32 },
    }
};
coex_schedule coex_schm_ble_mesh_standby_bt_default_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 3, .period = 2,
    .phases = {
        { .weight = 38, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 32, .bt_control_b = 4 },
        { .weight = 12, .wifi_control = 0, .bt_control_a = 2, .bt_control_b = 64 },
    }
};
coex_schedule coex_schm_ble_default_bt_idle_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 24,
    .phases = {
        { .weight = 50, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_default_bt_idle_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 20,
    .phases = {
        { .weight = 50, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_default_bt_idle_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 50, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_default_bt_a2dp_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 34,
    .phases = {
        { .weight = 30, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 70, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_default_bt_default_wifi_scan __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 30,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 60, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_default_bt_a2dp_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 33,
    .phases = {
        { .weight = 30, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 70, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_default_bt_default_wifi_connecting __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 25,
    .phases = {
        { .weight = 40, .wifi_control = 2, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 60, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_default_bt_a2dp_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 30, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 70, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_default_bt_default_wifi_conn __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 40, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 60, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_default_bt_a2dp_wifi_default __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 30, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 70, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_default_bt_idle_wifi_default __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 2, .period = 1,
    .phases = {
        { .weight = 50, .wifi_control = 3, .bt_control_a = 0, .bt_control_b = 0 },
        { .weight = 50, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
coex_schedule coex_schm_ble_idle_bt_idle_wifi_default __attribute__((weak, section(".dram1.policy"))) = {
    .phase_count = 1, .period = 1,
    .phases = {
        { .weight = 100, .wifi_control = 0, .bt_control_a = 0, .bt_control_b = 0 },
    }
};
uint8_t coex_pti_tab[42] __attribute__((section(".dram1.policy"))) = {
    [0] = 14,
    [1] = 10,
    [2] = 12,
    [3] = 12,
    [4] = 14,
    [5] = 1,
    [6] = 1,
    [7] = 1,
    [8] = 1,
    [9] = 10,
    [10] = 4,
    [11] = 3,
    [12] = 2,
    [13] = 1,
    [14] = 1,
    [15] = 1,
    [16] = 8,
    [17] = 10,
    [18] = 8,
    [19] = 8,
    [20] = 10,
    [21] = 8,
    [22] = 10,
    [23] = 8,
    [24] = 8,
    [25] = 10,
    [26] = 10,
    [27] = 10,
    [28] = 10,
    [29] = 8,
    [30] = 8,
    [31] = 8,
    [32] = 8,
    [33] = 3,
    [34] = 3,
    [35] = 3,
    [36] = 15,
    [37] = 14,
    [38] = 8,
    [39] = 14,
    [40] = 14,
    [41] = 15,
};
uint32_t g_coex_param[5] = {25000, 20000, 5000, 25000, 50000};
