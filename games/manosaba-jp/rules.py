# Rewriter rules for manosaba (JP) — keyed by NSO build ID.
CUTS = {
    # JP sdk — ab3835b2a37b1916e296f27e82dd4e36 (byte-identical to CN's sdk, copied verbatim)
    'ab3835b2a37b1916e296f27e82dd4e36': [
        (0x14fbf0, '08fca752', '087ca052', 'AddressRegionManager constructor: node area limit mov w8, #0x3fe00000 → mov w8, #0x3e00000'),
        (0x14fc5c, '09fca752', '097ca052', 'AddressRegionManager constructor: occupied node area, mov w9, #0x3fe00000 → mov w9, #0x3e00000'),
    ],
    # JP main — 0b4df9ef96ee8366af0d1086f9bdee9f
    '0b4df9ef96ee8366af0d1086f9bdee9f': [
        (0x20,      'e0031f2a',           '20008052',          'touchscreen player setting 0x20: mov w0, wzr → mov w0, #1'),
        # ProfilePage.ControlPresentEvidenceUi (CN 0x191d5b0): the JP compiler inlined IsConsole differently; no matching
        # `bl IsConsole; eor w8, w0, #1` site found. Cosmetic only (present-button TrueShadow on the profile page) — skipped.
        (0x2000f84, 'bf620694',           '00008052',          'WitchBookPageBase.ControlPresentEvidenceUi: bl IsConsole → mov w0, #0'),
        (0x3314b10, '5f00007268008052',   '011000d4c0035fd6',  'Screen.SetResolution → svc #0x80; ret'),
        (0x90b050,  '08f08052280000b9',   '211000d4c0035fd6',  'screen manager display size → svc #0x81; ret'),
        (0x99a380,  '08f08052280000b9',   '211000d4c0035fd6',  'graphics device display size → svc #0x81; ret'),
    ],
}
OBS = {
    '0b4df9ef96ee8366af0d1086f9bdee9f': [
        (0x71a380, 1, 'd10283ff', 'main loop step: resolution for the screen the window moved to (graphics.c step_gate)'),
    ],
}
PARAMS = {}
