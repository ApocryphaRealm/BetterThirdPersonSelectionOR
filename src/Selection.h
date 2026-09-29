#pragma once

// ============================================================================================================
// The selection (game thread, every frame): the references near the character that the player could use - items,
// containers, doors, activators, flora, furniture, people and creatures, each with a name - within RANGE of the
// character and within MAX ANGLE of where the camera looks. The best is the one closest to the aim, then the nearest;
// the game's own crosshair pick always wins when it has one, so this only widens the pick.
//
// It also watches the game's own pick chain in InterfaceManager (pickRef 0x140, reticleRef 0x148, crosshairRef 0x150,
// activateRef 0x158, fuzzyActivatePick 0x174) and logs every change: the in-game test reads that log to learn which
// field the prompt and the activation use. Until then [Test] iApplyTo=0 writes nothing; 1-5 write the choice into the
// field(s) named and log, the next frame, whether the game kept the write.
// ============================================================================================================

namespace selection
{
	void Tick();

	enum class Reason
	{
		kActive,        // selecting in this view
		kNotReady,      // no player or interface yet
		kOff,           // switched off
		kMenu,          // a menu is open
		kOffThird,      // off in third person
		kOffFirst,      // off in first person
		kNoCamera,      // calibrated, but the camera cannot be read now
		kCalibrating,   // the character has not walked far enough yet
	};

	struct Status
	{
		bool        active = false;   // selecting in this view (on, not in a menu, the view's switch on, calibrated)
		Reason      reason = Reason::kNotReady;
		std::string why;              // the reason in English, for the log and the tool
		std::string choice;           // this mod's pick, "" when none
		std::string game;             // the game's own pick, "" when none
		std::uint32_t candidates = 0;
		std::string applyTo;
	};
	Status GetStatus();   // any thread
	json   State();       // any thread: the TestBench tool's answer
}
