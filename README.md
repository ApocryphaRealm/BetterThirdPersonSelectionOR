# Better Third-Person Selection

An OBSE64 plugin for The Elder Scrolls IV: Oblivion Remastered: use what you are roughly looking at. The game only lets
you activate what the crosshair is exactly on; this widens the pick to everything within a reach of your character and
within an angle of where the camera looks, and hands the best of them to the game's own activation - its prompt, its
activate button, everything after that is the game's. A clean rebuild: Skyrim's Better Third Person Selection
(Shrimperator) is the reference for what it does, not for how.

**Test build - no version issued yet (rule 48).** The choice is written into InterfaceManager's `activateRef`, the field
the game's own prompt target lives in (read in game 2026-09-29), whenever the game's own pick is empty.

## What it does

* Every frame, in third person (first person keeps the game's precise pick unless switched on), it looks at the
  references in the cells around the character: items, containers, doors, activators, flora, furniture, people and
  creatures - each with a name, as the game prompts only for what has one.
* It keeps those within **Reach** (default 200 game units, about three metres) of the character and within **Angle**
  (default 60 degrees either side) of the camera's aim, and chooses the one closest to the aim, then the nearest.
* What the crosshair itself points at always wins; this only fills in when the game picked nothing.
* The camera is Unreal's, read from the player camera manager. Unreal's world and Oblivion's differ by a scale and
  maybe by swapped or mirrored axes, so neither is assumed: the first few steps the character walks calibrate them (the
  log names the result).

Settings: the page on the Apocrypha Menu Framework (switches for on, third person, first person; sliders for reach and
angle) and `BetterThirdPersonSelection.ini`. `[Test] iApplyTo` picks where the choice is written while testing: 0 nowhere, 1
pickRef, 2 reticleRef, 3 crosshairRef, 4 activateRef, 5 all four.

## TestBench

`selection.state`: op `state` (settings, this mod's pick and the game's, the best five candidates with angle, distance
and score, the game's pick fields, the camera and its calibration), op `set {key, value}` (enabled, thirdPerson,
firstPerson, range, maxAngle, applyTo, logTargets), op `reset`.

## Building

* [xmake](https://xmake.io) 3.0+, MSVC with C++23, and the submodule: `git clone --recurse-submodules`.
* `xmake f -p windows -a x64 -m releasedbg --toolchain=msvc`, then `xmake build BetterThirdPersonSelection`. Dear ImGui 1.90.8
  docking is vendored under `extern/imgui` - the framework's own version, which `AMF::UseFrameworkImGui()` checks.
* The plugin and `dist/OblivionRemastered/Binaries/Win64/OBSE/Plugins/*` go to `OblivionRemastered\Binaries\Win64\OBSE\Plugins\`
  (Mod Organizer 2: the Root Builder layout). The translations ship in the framework's translation folder.
* `python tools/translations.py` writes the eleven translation files.

## Licence

GPL-3.0-or-later (`LICENSE`); it links CommonLibOB64 (GPL-3.0). The modding exception from the CommonLibOB64 plugin
template is kept as `EXCEPTIONS`. `include/AMF.h` and `include/TestBenchAPI.h` are MIT.
