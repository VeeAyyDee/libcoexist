#!/usr/bin/env python3
# SPDX-License-Identifier: MPL-2.0
import hashlib
import json
from pathlib import Path
root = Path(__file__).resolve().parents[1]
inputs = [root / 'AGENTS.md'] + sorted((root / 'spec').rglob('*'))
record = {
    'implementation_license': 'MPL-2.0',
    'public_sdk_declarations_license': 'Apache-2.0 (retained notices)',
    'allowed_input_boundary': 'This libcoexist tree and standard host compiler/system headers only',
    'excluded_inputs': ['Original binaries/disassembly', 'Reconstructed source', 'Private evidence paths', 'Other implementation trees', 'Installed SDK sources'],
    'network_or_hardware_used_by_implementation_agent': False,
    'supervisor_build_only_contributions': {
        'feedback': ['spec/SUPERVISOR-FEEDBACK-01.txt', 'spec/SUPERVISOR-FEEDBACK-02.txt'],
        'changes': ['Absolute configure_file input path', 'Build-local ROM linker script removing 14 conflicting absolute function assignments'],
        'runtime_c_changed': False,
        'installed_sdk_modified': False,
        'target_pin': 'ESP32-S3 / ESP-IDF 6.0.3',
        'sdk_rom_script_sha256': '67b760f9ac1736bc5ab4182508ea5d3197e0908b6f2d3ae39f29fc853b9fbea9',
        'build_files_sha256': {name: hashlib.sha256((root / name).read_bytes()).hexdigest() for name in ['CMakeLists.txt', 'cmake/replace_coexist.cmake', 'cmake/replace_rom_bindings.cmake']}
    },
    'inputs_sha256': {str(p.relative_to(root)): hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs if p.is_file()},
    'verification': {
        'host': 'make test: strict compiler; ASan/UBSan core and full integration suites; strong weak-policy override suite',
        'layout': 'Freestanding -m32 compile; actual target compiler check built into CMake component',
        'exports': 'Exact 89 functions + 117 objects; 107 weak descriptors; descriptor sizes checked',
        'leak_sanitizer': 'Disabled: incompatible with ptrace sandbox; no leak-freedom claim',
        'target_sdk_and_hardware': 'Supervisor-owned qualification, not performed by implementation agent',
        'whole_binary_equivalence': 'Not claimed from host functional-contract tests or bounded independent comparisons',
        'supervisor_independent_comparison': {
            'source': 'spec/SUPERVISOR-FEEDBACK-02.txt',
            'result': 'PASS', 'selector_cases': 256, 'mmio_cases': 68,
            'ordered_mmio_transactions': 314,
            'oracle_inputs_read_by_implementation_agent': False
        },
        'final_target_relink_ownership_placement_audit': 'Pending supervisor validation at this documentation revision'
    },
    'compatibility_choices': [
        'Preserve one-byte counter wrap and observed thresholds',
        'Preserve deleted core semaphore handle after scheduler-init failure',
        'Preserve ROM dispatch publication-before-fill and free-before-clear with caller quiescence requirement',
        'Preserve negative phase index integer-address behavior; outside supported caller domain',
        'Keep all independently addressed weak policies, including equal-valued descriptors',
        'Use normalized internal ABI declarations from closure 05 and public SDK declarations',
        'Use original diagnostic wording under closure 05 normalization allowance'
    ]
}
(root / 'provenance.json').write_text(json.dumps(record, indent=2) + '\n')
