# BetterSelectionOR - changelog

Written as changes happen, not reconstructed afterwards (rule 61). A version number is issued by the version gate
only once a build is seen working in game (rule 48); until then the work sits under Unreleased.

## Unreleased - 2026-09-29 - untested

### Added
- the owner, 2026-09-29: "All that BTPS really does is expand the area that the crosshair affects when selecting items.
  And the angle at which it selects them." The references in the cells around the character that the player could use
  (named items, containers, doors, activators, flora, furniture, people and creatures) within Reach of the character and
  Angle of the camera's aim; the closest to the aim wins, then the nearest; the game's own crosshair pick always wins.
- the camera read from Unreal's player camera manager (GetCameraLocation / GetCameraRotation through ProcessEvent, no
  hook), converted to Oblivion's world by a calibration learned from the character's first steps (axis arrangement
  and scale, logged).
- the probe the plan asks for: every change of InterfaceManager's pickRef / reticleRef / crosshairRef / activateRef /
  telekinesisRef / fuzzyActivatePick / pickDistance is logged ([Test] bLogTargets=1), beside this mod's own choice.
- [Test] iApplyTo: 0 (the default) writes nothing; 1-5 write the choice into the named field(s) where the game picked
  nothing, and the log says the next frame whether the game kept the write.
- the Apocrypha Menu Framework page "Better Selection": switches (on, third person, first person), Reach and Angle
  sliders, a reset, a live status line; eleven languages.
- TestBench tool selection.state (state, set, reset).

### Known
- not run in game yet. Open questions for the test: whether the calibration settles (the log's "camera: calibrated"
  line); whether the exterior cell look-up finds the neighbouring cells (debug log); which field the prompt and the
  activation read, and whether a write from the frame tick survives to the activation.
