// Better Third-Person Selection for Oblivion Remastered - use what you are roughly looking at: the pick widened to a range around
// the character and an angle around the camera's aim, handed to the game's own activation. A clean rebuild: Skyrim's
// Better Third Person Selection (Shrimperator) is the reference for what it does, not for how.
// Plan: 4. plans\better-selection-oblivion\PLAN.md.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Page.h"
#include "Selection.h"
#include "Settings.h"
#include "Tick.h"

namespace tool { bool Register(); }

namespace
{
	// every frame (from the message pump, on the game thread)
	void OnFrame()
	{
		static bool dataLoaded = false;
		static bool toolRegistered = false;
		static std::uint64_t n = 0;
		++n;
		if (!toolRegistered && n % 60 == 0) {
			toolRegistered = tool::Register();
		}
		if (!dataLoaded) {
			// the player and the interface exist only once a game is running; nothing is looked at before that
			dataLoaded = RE::PlayerCharacter::GetSingleton() != nullptr;
			if (!dataLoaded) {
				return;
			}
			logger::info("the player exists - selection starts");
		}
		selection::Tick();
	}

	void OnMessage(OBSE::MessagingInterface::Message* a_msg)
	{
		if (!a_msg) {
			return;
		}
		switch (a_msg->type) {
		case OBSE::MessagingInterface::kPostLoad:
			tick::Install(&OnFrame);
			page::Register();
			break;
		default:
			break;
		}
	}
}

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* a_obse)
{
	OBSE::Init(a_obse);
	settings::Load();
	const auto v = settings::Snapshot();
	const auto level = static_cast<spdlog::level::level_enum>(std::clamp(v.logLevel, 0, 6));
	logger::set_level(level, level);
	// rule 14: the log names its level and how to get everything
	logger::info("Better Third-Person Selection {} loaded (Oblivion Remastered) - log level {}; set uLogLevel=0 in BetterThirdPersonSelection.ini to capture everything",
		SEL_VERSION, v.logLevel);
	if (auto* messaging = OBSE::GetMessagingInterface(); !messaging || !messaging->RegisterListener(&OnMessage)) {
		logger::error("OBSE messaging unavailable - no frame tick, nothing is selected");
	}
	return true;
}
