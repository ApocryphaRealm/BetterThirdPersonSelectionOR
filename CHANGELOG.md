# BetterThirdPersonSelectionOR - changelog

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
- the Apocrypha Menu Framework page "Better Third-Person Selection": switches (on, third person, first person), Reach and Angle
  sliders, a reset, a live status line; eleven languages.
- TestBench tool selection.state (state, set, reset).

### Fixed (after the first round, 09:12)
- the frame stopped at "a menu is open" in gameplay: Oblivion Remastered's InterfaceManager::menuMode is 1 while
  playing, and the tick tested it for truth - so the camera was never read (the round's log had the pick lines and
  nothing after). Now menuMode != 1 is a menu, as Tween Menu and Improved Wheel Menu read it.
- the cached player controller is held with its object-array slot and checked there, never by reading the pointer (the
  same freed-memory read that crashed Improved Wheel Menu at 09:15).
- each stop reason ("state: ...", "camera: not read - ...") is logged once per change at info, so a round at the shipped
  log level shows which stage it reached.

### Known
- first round (09:12): loaded, page and tool registered; the five pick fields were empty on every change logged (pick
  distance 128 / 150 / 80) - the owner was mostly in the inventory, so it is not yet known whether the game fills them
  at all in Oblivion Remastered. If they stay empty while the game shows a prompt, the prompt is Unreal-side and the
  write target has to be found there instead.
- not run in game past that yet. Open questions for the test: whether the calibration settles (the log's "camera: calibrated"
  line); whether the exterior cell look-up finds the neighbouring cells (debug log); which field the prompt and the
  activation read, and whether a write from the frame tick survives to the activation.
