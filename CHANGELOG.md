# BetterThirdPersonSelectionOR - changelog

Written as changes happen, not reconstructed afterwards (rule 61). A version number is issued by the version gate
only once a build is seen working in game (rule 48); until then the work sits under Unreleased.

## 1.0.0 - 2026-09-29 - working

The first release (the owner: "lets default btps 35 degrees angle and 300 reach and finalize it"): defaults Reach 300,
Angle 35 (the page allows 50-400 and 5-75); the per-change target log off by default, the write-check summary at debug.
Everything below is how it got here, in the order it happened.

## Before 1.0.0 - 2026-09-29 - test builds

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

### Changed (after round 7 - no crash, but a bench under the map could still be sat on at Reach 600 / Angle 89)
- the owner: "we just set default values to be reasonable". The defaults stay Skyrim BTPS's own (Reach 200 units, about
  three metres; Angle 60 degrees); the page's ceilings come down from 600 / 90 to 400 / 75 (the INI is clamped the
  same), and the owner's installed INI was put back to the defaults.

### Fixed (after round 6 - the owner: "i selected a bench under the ground and it crashed", 11:32:40)
- the crash: the fallback called the engine's TESForm::Activate on a bench (292 units away, under the ground - the game
  itself had refused it) from the frame tick, which runs on the UE game thread; Oblivion Remastered traps any change of a
  reference's state off the TES simulation thread (a null write after a thread-id compare - the primary agent's reading
  of the crash record; logic library "equipment path traps every thread but the TES thread"). The fallback is REMOVED:
  the game's own activation already acts on the activateRef write (round 5), so a press is only watched and logged
  ("the game did not act - left alone" when it refuses).
- a vertical reach: a reference more than 100 units under the character's feet or 250 above them is not chosen.
- the write check is one summary line every 5 s (it flipped every frame and filled the log).
- null guards audited (rule 14 - the owner: "its standard practice to build with null guards at every relavent
  point"): every class, class default object and cell this mod dereferences is checked first.
- the marker's text is white (round 6 confirmed): the colour now goes on after the prefab's own style is applied.

### Fixed (after round 5 - the owner saw the marker)
- the marker showed the name's localisation KEY ("a really long name"): Oblivion Remastered's full names are keys into
  the string table ST_FullNames (Content/Localization/StringTables); a LOC_ key now goes through
  KismetTextLibrary::TextFromStringTable, as the game's own UI shows it, in the game's language.
- the marker's text was black (the prefab's colour): now white with a soft dark shadow; and drawn at 80% of the
  prefab's size ("a little too big").

### Known (round 5, 11:19-11:21)
- the game's own activation reads the activateRef write: an Activate press on the chosen Iron Arrow was taken by the
  game within 94 ms ("left the world - the game handled the press"); on a guard, dialogue opened. The fallback did not
  have to act. Keys seen: IA_Game_Actions_Activate on E and Gamepad_FaceButton_Bottom.

### Added (after round 4 - the owner: "BTPS did work just without a marker")
- the marker: the chosen reference's name, in the game's own HUD text (the text prefab WBP_AltarTextBlock), drawn just
  above the reference where it stands - over a person's head, on top of a container or door, just above an item - so the
  reach and the angle can be seen (the owner: "build the activation prompt for wherever the item is at so that I know
  that it's capable of reaching that distance"). Shown only when this mod's choice is what Activate will use; the game's
  own pick keeps the game's own prompt. Built once like Tween Menu's menu (a canvas on the viewport, then only shown and
  hidden), placed by UMG's own projection (WidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition) of the reference's
  place in Unreal's world (camera::ToUnreal, from the calibration). [General] bShowMarker=1 and a switch on the page
  ("Show what will be used, where it is", eleven languages); selection.state reports it.

### Fixed (after round 3 - the owner: "btps still does nothing")
- the fallback never saw an Activate press: it looked for IA_Game_Default_Activate in IMC_Game_Default, which does not
  exist. The game's packaged asset list has the action as Content/Dev/Input/GamePlay/InputActions/Actions/
  IA_Game_Actions_Activate, mapped in IMC_Game_Actions; that context is searched first, IMC_Game_Default after it.
- the fallback decides by outcome instead of by the write check: after a press on the choice it waits 200 ms, and if
  the game neither took the item (the reference, looked up again by form ID, is gone, deleted, disabled or out of its
  cell) nor opened a menu (container, dialogue, loading), the mod activates it. A door or an activator leaves no trace to
  wait for, so for those a kept write is still left to the game (a door toggled twice would close again).
- the previous launch's log is kept as BetterThirdPersonSelection.prev.log: round 3's log was overwritten by a relaunch
  that crashed in UE4SS's Lua loader (Ultimate Combat Redux starting, 11:00:47 - not this mod).

### Added (before round 3)
- the owner, 2026-09-29: "figure out what went wrong with BTPS because it didn't do anything". Round 2 ran the
  observe-only build (its log: "apply to observe", no write lines) - it never handed anything to the game. The risk left
  for the activateRef build: the write happens at the START of the frame (the message pump) and the game recomputes
  activateRef during the frame, so its prompt and its activation may never see the choice. So the Activate press has a
  fallback that does not depend on that timing: the keys of IA_Game_Default_Activate in IMC_Game_Default are watched
  (IsInputKeyDown edges, rebinds followed; every mapped action with "Activate" in its name is logged once, since the
  action name is not verified yet), and on a press with the game's own pick empty and the write check saying the game
  REPLACED the write, the choice is activated through the engine's own TESForm::Activate (the pick's own call). While
  the write is kept, the press is left to the game - one press never activates twice. selection.state reports the
  action, its keys, the presses and the fallbacks run.

### Changed (after round 2, 09:47-09:50)
- the choice is now written into InterfaceManager::activateRef by default ([Test] iApplyTo=4): round 2 showed the game's
  own prompt target there alone - it held the Iron Arrow whenever the prompt showed, while pickRef, reticleRef,
  crosshairRef and telekinesisRef stayed empty. The calibration settled on the fifth step (UE x = +obl.x, UE y = -obl.y,
  1.4288 Unreal units per Oblivion unit, agreement 1.000), and the selection found the arrow at up to 58 degrees and 199
  units. Round 2 wrote nothing (the observe-only build), which is why the owner saw no change in game.
- the view switch is debounced (150 ms): is3rdPerson reads false for one frame every few seconds in third person.

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
