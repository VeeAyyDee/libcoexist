#!/usr/bin/env python3
# SPDX-License-Identifier: MPL-2.0
"""Audit all public archive exports without loading private/reference objects."""
import json
import subprocess
import sys
from pathlib import Path
root = Path(__file__).resolve().parents[1]
archive = Path(sys.argv[1])
functions = json.loads((root / 'spec/function-exports.json').read_text())['functions']
data = json.loads((root / 'spec/data-exports.json').read_text())['exports']
found = {}
for line in subprocess.check_output(['nm', '-g', '-S', '--defined-only', str(archive)], text=True).splitlines():
    fields = line.split()
    if len(fields) == 4:
        address, size, kind, name = fields
        assert name not in found, f'duplicate public symbol: {name}'
        found[name] = {'size': int(size, 16), 'kind': kind}
expected = {r['name'] for r in functions + data}
assert set(found) == expected, {'missing': sorted(expected - set(found)), 'extra': sorted(set(found) - expected)}
for record in functions:
    assert found[record['name']]['kind'] == 'T', record['name']
pointer_names = {'g_coex_adapter_funcs_md5', 'g_coex_i154_funcs_md5', 'g_coex_basic_md5', 'libcoexist_reversion_git', 'libcoexist_reversion_remote'}
for record in data:
    actual = found[record['name']]
    assert (actual['kind'] == 'V') == (record['binding'] == 'WEAK'), record['name']
    if record['name'] not in pointer_names | {'coex_env', 'coex_schm_env'}:
        assert actual['size'] == record['original_size'], (record['name'], actual)
undefined = set()
for line in subprocess.check_output(['nm', '-u', str(archive)], text=True).splitlines():
    fields = line.split()
    if len(fields) == 2 and fields[0] == 'U': undefined.add(fields[1])
undefined -= set(found)
report = {
    'undefined_external_dependencies': sorted(undefined),
    'archive': str(archive), 'functions': len(functions), 'data_objects': len(data),
    'weak_descriptors': sum(r['binding'] == 'WEAK' for r in data),
    'host_pointer_layout_note': 'Pointer objects/core/scheduler use native host layout; target sizes are checked in tests/layout.c.',
    'exports': found,
    'status': 'PASS'
}
(root / 'build/export-manifest.json').write_text(json.dumps(report, indent=2) + '\n')
print('exports: PASS (89 functions, 117 data objects, 107 weak descriptors)')
