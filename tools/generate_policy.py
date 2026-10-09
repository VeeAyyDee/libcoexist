#!/usr/bin/env python3
# SPDX-License-Identifier: MPL-2.0
"""Express the supervisor's named functional facts as original typed objects."""
import json
from pathlib import Path
root = Path(__file__).resolve().parents[1]
facts = json.loads((root / 'spec/01-policy-defaults.json').read_text())
inventory = {x['name']: x for x in json.loads((root / 'spec/data-exports.json').read_text())['exports']}
lines = ['/* SPDX-License-Identifier: MPL-2.0 */', '/* Generated from named facts by tools/generate_policy.py. */', '#include "coexist_subset.h"']
header = ['/* SPDX-License-Identifier: MPL-2.0 */', '#pragma once', '#include "coexist_subset.h"']
for record in facts['descriptor_defaults']:
    name, phases = record['symbol'], record['phases_in_order']
    assert len(phases) == record['phase_count']
    assert sum(p['duration_weight'] for p in phases) == 100
    assert inventory[name]['original_size'] == 2 + 4 * len(phases)
    header.append(f'extern coex_schedule {name};')
    lines.extend([f'coex_schedule {name} __attribute__((weak, section(".dram1.policy"))) = {{', f'    .phase_count = {len(phases)}, .period = {record["period_multiplier"]},', '    .phases = {'])
    for p in phases:
        lines.append('        { .weight = %(duration_weight)d, .wifi_control = %(wifi_control)d, .bt_control_a = %(bluetooth_control_a)d, .bt_control_b = %(bluetooth_control_b)d },' % p)
    lines.extend(['    }', '};'])
priorities = {}
for group in facts['priority_defaults_grouped_by_value']:
    for event in group['events']:
        assert event not in priorities
        priorities[event] = group['priority']
assert set(priorities) == set(range(42))
lines.append('uint8_t coex_pti_tab[42] __attribute__((section(".dram1.policy"))) = {')
lines.extend(f'    [{event}] = {value},' for event, value in sorted(priorities.items()))
lines.append('};')
assert [r['event'] for r in facts['duration_defaults']] == [4, 7, 9, 39, 40]
lines.append('uint32_t g_coex_param[5] = {' + ', '.join(str(r['duration']) for r in facts['duration_defaults']) + '};')
(root / 'src/policy.c').write_text('\n'.join(lines) + '\n')
(root / 'include/coexist_policy.h').write_text('\n'.join(header) + '\n')
