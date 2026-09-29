#pragma once

// The mod's page on the Apocrypha Menu Framework: on/off, third person / first person switches, range and angle
// sliders, and a live line naming what is selected. Drawn on AMF's render thread, so it never touches a game form: it
// reads selection::GetStatus() and changes settings through settings::Update().

namespace page
{
	inline constexpr const char* kModName = "Better Selection";
	void Register();
}
