#pragma once

// ============================================================================================================
// The activate press, and the fallback that makes the choice usable when the game will not read it (game thread).
//
// The choice is written into InterfaceManager::activateRef every frame, from the frame tick at the START of the frame.
// The game recomputes that field during the frame (round 2's log: it flips between the item and empty as the aim
// moves), so the write may be gone again before its prompt and its activation read it. The fallback does not depend
// on that timing: the keys the game's Activate action has in IMC_Game_Default are watched (IsInputKeyDown edges, as
// Tween Menu watches its own), and on a press with the game's own pick empty, the chosen reference is activated
// through the engine's own TESForm::Activate - but ONLY while the write-check says the game replaced the write, so
// one press can never activate twice.
// ============================================================================================================

namespace activate
{
	// true on the tick the Activate action's key went down (keyboard or pad, following rebinds)
	bool PressedThisTick(UE::UObject* a_playerController);

	// activates a_ref as the player would; logs what happened
	void Run(RE::TESObjectREFR* a_ref, RE::PlayerCharacter* a_player);

	json State();   // the action, its keys, presses, fallbacks run (any thread)
}
