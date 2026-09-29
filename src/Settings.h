#pragma once

// BetterSelection.ini beside the plugin. The compiled defaults are the shipped INI's values (rule 16); Save() rewrites
// only this plugin's keys in place with ordinary file I/O (never WritePrivateProfileString), so comments and unknown
// lines survive and a change on the page persists. The page draws on AMF's render thread and the selection runs on
// the game thread, so the values are read and written only through Snapshot() / Update() under one lock.

namespace settings
{
	// Where the chosen reference is written. The in-game test decides which field the game's prompt and activation
	// read (plan: 4. plans\better-selection-oblivion\PLAN.md, "Probe first"); until then the mod only observes.
	enum ApplyTo : int
	{
		kObserve = 0,        // write nothing: log what the game picked and what this mod would pick
		kPickRef = 1,        // InterfaceManager 0x140
		kReticleRef = 2,     // 0x148
		kCrosshairRef = 3,   // 0x150
		kActivateRef = 4,    // 0x158
		kAllFour = 5,
		kApplyToCount
	};

	struct Values
	{
		bool  enabled = true;       // [General] bEnabled
		bool  thirdPerson = true;   // [General] bThirdPerson
		bool  firstPerson = false;  // [General] bFirstPerson (first person keeps the game's own precise pick)
		float range = 200.0f;       // [General] fRange - game units from the character
		float maxAngle = 60.0f;     // [General] fMaxAngle - degrees either side of where the camera looks
		int   applyTo = kObserve;   // [Test] iApplyTo
		bool  logTargets = true;    // [Test] bLogTargets - one log line whenever the game's or this mod's target changes
		int   logLevel = 2;         // [Log] uLogLevel (rule 14: shipped at info)
	};

	inline constexpr float kRangeMin = 50.0f, kRangeMax = 600.0f;
	inline constexpr float kAngleMin = 5.0f, kAngleMax = 90.0f;

	Values Snapshot();
	void   Update(const std::function<void(Values&)>& a_change);   // clamps, then saves
	void   Reset();                                                   // the compiled defaults, saved

	void                  Load();
	bool                  Save();
	std::filesystem::path PluginFolder();   // ...\OblivionRemastered\Binaries\Win64\OBSE\Plugins
	std::filesystem::path IniPath();
	const char*           ApplyToName(int a_applyTo);
}
