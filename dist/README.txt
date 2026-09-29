Better Third-Person Selection
=============================
Version 1.0.0

An original, GPL-3.0-or-later OBSE64 plugin for The Elder Scrolls IV: Oblivion Remastered. In third
person you no longer have to put the crosshair exactly on something to use it: anything within reach
of your character and roughly where the camera looks can be picked up, opened, talked to or sat on.
Its name is shown right where it stands, so you always see what Activate will use.

HOW IT WORKS
------------
  * It looks at what you could use around your character - items, containers, doors, activators,
    plants, furniture, people and creatures, anything with a name - and keeps what is within Reach of
    your character and within Angle of where the camera looks.
  * Of those, the one closest to where you look wins, then the nearest. What the crosshair itself is
    on always wins, and the game shows its own prompt for it.
  * The chosen thing's name appears just above it, in the game's own HUD text. Press Activate and the
    game uses it exactly as if you had aimed at it - the game does the activating, not this mod.
  * Things far below your feet or high above your head are left out.
  * First person keeps the game's own precise aiming unless you switch it on.

SETTINGS
--------
Open the Apocrypha Menu Framework (F1), then Better Third-Person Selection:
  * Wider selection - on / off
  * In third person / In first person
  * Show what will be used, where it is - the name marker
  * Reach - how far from your character, in game units (about 70 to a metre), 50 to 400.
    Default 300.
  * Angle - how far to either side of where the camera looks, in degrees, 5 to 75. Default 35.
Changes apply at once and are saved to BetterThirdPersonSelection.ini. Very large values reach things
the game would never let you use; the defaults are a good start.

REQUIREMENTS
------------
  * The Elder Scrolls IV: Oblivion Remastered (Steam, runtime 1.512.105)
  * OBSE64 (Oblivion Script Extender 64)
  * Address Library for OBSE Plugins
  * Apocrypha Menu Framework for Oblivion Remastered - for the settings page (without it the INI
    still applies)

INSTALLING
----------
The plugin goes beside the game executable, in
OblivionRemastered\Binaries\Win64\OBSE\Plugins\. With Mod Organizer 2 that means the Root
folder layout (Root Builder); launch the game through OBSE64.

FILES
-----
  * OBSE/Plugins/BetterThirdPersonSelection.dll - the plugin (BetterThirdPersonSelection.pdb, its
    debug symbols, beside it)
  * OBSE/Plugins/BetterThirdPersonSelection.ini - the settings and the log level
  * OBSE/Plugins/ApocryphaMenuFramework/Translations/BetterThirdPersonSelection_*.txt - the page in
    eleven languages
  * The log is Documents/My Games/Oblivion Remastered/OBSE/Logs/BetterThirdPersonSelection.log (the
    previous game's log is kept beside it as BetterThirdPersonSelection.prev.log). It is written at
    info; set uLogLevel=0 in the INI for everything when reporting a problem.

CREDIT AND LICENCE
------------------
The idea comes from Better Third Person Selection for Skyrim by Shrimperator; this is a clean
rebuild for Oblivion Remastered with none of its code or files. GPL-3.0-or-later (LICENSE,
NOTICE.md); the components it links and their notices: THIRD_PARTY_NOTICES.md.
