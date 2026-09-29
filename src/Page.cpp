#include "Page.h"

#include <imgui.h>

#include "AMF.h"
#include "Selection.h"
#include "Settings.h"
#include "Strings.h"

namespace page
{
	namespace
	{
		// An on/off switch (rule 32 - never a checkbox): the framework's own design - a red/green track and a white
		// knob (ApocryphaMenuFrameworkOR include/utils/ToggleSwitch.h), drawn in fixed colours because AMF's theme
		// leaves Button and FrameBg transparent (a theme-coloured track shows only its knob there).
		bool Switch(const char* a_label, bool* a_v)
		{
			ImGui::PushID(a_label);
			const float  h = ImGui::GetFrameHeight();
			const float  w = h * 2.0f;
			const float  r = h * 0.5f;
			const ImVec2 p = ImGui::GetCursorScreenPos();
			const bool   pressed = ImGui::InvisibleButton("##switch", ImVec2(w, h));
			if (pressed) *a_v = !*a_v;
			const bool hot = ImGui::IsItemHovered() || ImGui::IsItemFocused();
			const ImU32 track = *a_v ? (hot ? IM_COL32(92, 191, 96, 255) : IM_COL32(76, 175, 80, 255))
			                         : (hot ? IM_COL32(207, 84, 84, 255) : IM_COL32(191, 68, 68, 255));
			auto* dl = ImGui::GetWindowDrawList();
			dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), track, r);
			dl->AddCircleFilled(ImVec2(p.x + r + (*a_v ? w - h : 0.0f), p.y + r), r - 2.0f, IM_COL32(240, 240, 240, 255), 32);
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted(a_label);
			ImGui::PopID();
			return pressed;
		}

		const char* ReasonText(selection::Reason a_reason)
		{
			using enum selection::Reason;
			switch (a_reason) {
			case kOff: return TR("SEL_StateOff", "Wider selection is off.");
			case kMenu: return TR("SEL_StateMenu", "Paused while a menu is open.");
			case kOffThird: return TR("SEL_StateOffThird", "Off in third person.");
			case kOffFirst: return TR("SEL_StateOffFirst", "Off in first person.");
			case kNoCamera: return TR("SEL_StateNoCamera", "The camera cannot be read right now.");
			case kCalibrating: return TR("SEL_StateCalibrating", "Getting ready - walk a few steps.");
			default: return TR("SEL_StateNotReady", "Waiting for the game.");
			}
		}

		void Draw()
		{
			if (!AMF::UseFrameworkImGui()) {
				return;
			}
			strings::Refresh();
			auto v = settings::Snapshot();
			ImGui::TextWrapped("%s", TR("SEL_Intro", "Use what you are roughly looking at: anything within reach and within the angle below counts, the "
													 "closest to where you look first. What the crosshair itself points at always wins."));
			ImGui::Spacing();

			ImGui::SeparatorText(TR("SEL_GroupWhen", "When"));
			if (Switch(TR("SEL_Enabled", "Wider selection"), &v.enabled)) {
				settings::Update([&](settings::Values& s) { s.enabled = v.enabled; });
				logger::info("page: wider selection {}", v.enabled ? "on" : "off");
			}
			if (Switch(TR("SEL_ThirdPerson", "In third person"), &v.thirdPerson)) {
				settings::Update([&](settings::Values& s) { s.thirdPerson = v.thirdPerson; });
				logger::info("page: third person {}", v.thirdPerson ? "on" : "off");
			}
			if (Switch(TR("SEL_FirstPerson", "In first person"), &v.firstPerson)) {
				settings::Update([&](settings::Values& s) { s.firstPerson = v.firstPerson; });
				logger::info("page: first person {}", v.firstPerson ? "on" : "off");
			}

			if (Switch(TR("SEL_Marker", "Show what will be used, where it is"), &v.showMarker)) {
				settings::Update([&](settings::Values& s) { s.showMarker = v.showMarker; });
				logger::info("page: marker {}", v.showMarker ? "on" : "off");
			}

			ImGui::SeparatorText(TR("SEL_GroupArea", "Area"));
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.6f);
			if (ImGui::SliderFloat(TR("SEL_Range", "Reach"), &v.range, settings::kRangeMin, settings::kRangeMax, "%.0f")) {
				settings::Update([&](settings::Values& s) { s.range = v.range; });
			}
			ImGui::TextDisabled("%s", TR("SEL_RangeHint", "How far from your character, in game units (about 70 units to a metre)."));
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.6f);
			if (ImGui::SliderFloat(TR("SEL_Angle", "Angle"), &v.maxAngle, settings::kAngleMin, settings::kAngleMax, "%.0f")) {
				settings::Update([&](settings::Values& s) { s.maxAngle = v.maxAngle; });
			}
			ImGui::TextDisabled("%s", TR("SEL_AngleHint", "How far to either side of where the camera looks, in degrees."));

			ImGui::Spacing();
			ImGui::Separator();
			if (ImGui::Button(TR("SEL_Reset", "Reset to the defaults"))) {
				settings::Reset();
				logger::info("page: reset to the defaults");
			}
			const auto st = selection::GetStatus();
			if (!st.active) {
				ImGui::TextDisabled("%s", ReasonText(st.reason));
			} else if (!st.choice.empty()) {
				ImGui::TextDisabled(TR("SEL_Selected", "Selected: %s"), st.choice.c_str());
			} else {
				ImGui::TextDisabled("%s", TR("SEL_Nothing", "Nothing within reach."));
			}
		}
	}

	void Register()
	{
		if (!AMF::IsInstalled()) {
			logger::info("the Apocrypha Menu Framework is not installed - no settings page (the INI still applies)");
			return;
		}
		if (AMF::RegisterPage(kModName, "Settings", &Draw)) {
			logger::info("AMF {}: the {} settings page is registered", AMF::Version(), kModName);
		} else {
			logger::warn("AMF refused the {} page", kModName);
		}
	}
}
