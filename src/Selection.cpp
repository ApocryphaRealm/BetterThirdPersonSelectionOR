#include "Selection.h"

#include "Camera.h"
#include "Settings.h"

namespace selection
{
	namespace
	{
		struct Candidate
		{
			RE::TESObjectREFR* ref = nullptr;
			float              angle = 0;   // degrees from the camera's aim
			float              dist = 0;    // game units from the character
			float              score = 0;   // lower is better
		};

		std::mutex             g_lock;   // g_status and g_top, read by the page and the tool on other threads
		Status                 g_status;
		std::vector<json>      g_top;    // the best few candidates, described
		json                   g_gameFields = json::object();
		RE::TESObjectREFR*     g_written = nullptr;   // what this mod wrote last frame (nullptr: nothing)
		int                    g_writtenTo = settings::kObserve;
		std::atomic<std::uint32_t> g_cellsScanned{ 0 };

		constexpr float kCellSize = 4096.0f;

		std::string Name(RE::TESObjectREFR* a_ref)
		{
			if (!a_ref) {
				return {};
			}
			auto*       base = a_ref->data.objectReference;
			const char* n = base ? RE::TESFullName::GetFullName(base) : nullptr;
			return std::format("{} [{:08X}]", n && *n ? n : "(no name)", a_ref->GetFormID());
		}

		// the reference's own kind when the player could use it; FormType::None when not
		RE::FormType UsableKind(RE::TESObjectREFR* a_ref, RE::TESObjectREFR* a_playerRef)
		{
			if (!a_ref || a_ref == a_playerRef || a_ref->IsDeleted() || (a_ref->GetFormFlags() & RE::TESForm::RecordFlags::kDisabled)) {
				return RE::FormType::None;
			}
			auto* base = a_ref->data.objectReference;
			if (!base) {
				return RE::FormType::None;
			}
			using enum RE::FormType;
			const auto type = base->GetFormType();
			switch (type) {
			case Apparatus:
			case Armor:
			case Book:
			case Clothing:
			case Ingredient:
			case Light:   // only a named light (a torch you can carry); fixtures carry no name
			case Misc:
			case Weapon:
			case Ammo:
			case SoulGem:
			case KeyMaster:
			case AlchemyItem:
			case SigilStone:
			case Container:
			case Door:
			case Activator:
			case Flora:
			case Furniture:
			case NPC:
			case Creature:
				break;
			default:
				return None;
			}
			const char* n = RE::TESFullName::GetFullName(base);
			return n && *n ? type : None;   // the game prompts only for what has a name
		}

		// the references in the cells around the character: the player's cell inside, the 3 x 3 around it outside
		std::vector<RE::TESObjectCELL*> NearbyCells(RE::PlayerCharacter* a_player)
		{
			std::vector<RE::TESObjectCELL*> cells;
			auto* cell = a_player->parentCell;
			if (!cell) {
				return cells;
			}
			cells.push_back(cell);
			if (a_player->GetInterior()) {
				return cells;
			}
			auto* world = a_player->GetWorldSpace();
			if (!world || !world->cellMap) {
				return cells;
			}
			const auto& p = a_player->data.location;
			const int   cx = static_cast<int>(std::floor(p.x / kCellSize)), cy = static_cast<int>(std::floor(p.y / kCellSize));
			std::uint32_t found = 0;
			for (int dx = -1; dx <= 1; ++dx) {
				for (int dy = -1; dy <= 1; ++dy) {
					// Oblivion's worldspace cell map key: x in the high half, y in the low half
					const std::int32_t key = static_cast<std::int32_t>((static_cast<std::uint32_t>(cx + dx) << 16) | (static_cast<std::uint32_t>(cy + dy) & 0xFFFF));
					const auto it = world->cellMap->find(key);
					if (it == world->cellMap->end() || !it->second) {
						continue;
					}
					++found;
					if (std::ranges::find(cells, it->second) == cells.end()) {
						cells.push_back(it->second);
					}
				}
			}
			static int lastFound = -1;
			if (static_cast<int>(found) != lastFound) {
				lastFound = static_cast<int>(found);
				logger::debug("selection: outside at cell ({}, {}) - {} of the 9 cells around found in the worldspace's cell map", cx, cy, found);
			}
			return cells;
		}

		std::vector<Candidate> Gather(RE::PlayerCharacter* a_player, const camera::View& a_view, const settings::Values& a_s)
		{
			std::vector<Candidate> out;
			const auto  cells = NearbyCells(a_player);
			const auto& me = a_player->data.location;
			const float cosMax = std::cos(a_s.maxAngle * std::numbers::pi_v<float> / 180.0f);
			g_cellsScanned = static_cast<std::uint32_t>(cells.size());
			for (auto* cell : cells) {
				for (auto* ref : cell->listReferences) {
					const auto kind = UsableKind(ref, a_player);
					if (kind == RE::FormType::None) {
						continue;
					}
					const auto& at = ref->data.location;
					const float dx = at.x - me.x, dy = at.y - me.y, dz = at.z - me.z;
					const float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
					if (dist > a_s.range) {
						continue;
					}
					// aim at the middle of a person or creature, at an item where it lies
					const bool  actor = kind == RE::FormType::NPC || kind == RE::FormType::Creature;
					const float tx = at.x - a_view.eye.x, ty = at.y - a_view.eye.y, tz = at.z + (actor ? 60.0f : 4.0f) - a_view.eye.z;
					const float tl = std::sqrt(tx * tx + ty * ty + tz * tz);
					if (tl < 1.0f) {
						continue;
					}
					const float c = (tx * a_view.fwd.x + ty * a_view.fwd.y + tz * a_view.fwd.z) / tl;
					if (c < cosMax) {
						continue;
					}
					const float angle = std::acos(std::clamp(c, -1.0f, 1.0f)) * 180.0f / std::numbers::pi_v<float>;
					out.push_back({ ref, angle, dist, angle / a_s.maxAngle + 0.35f * dist / a_s.range });
				}
			}
			std::ranges::sort(out, {}, &Candidate::score);
			return out;
		}

		RE::TESObjectREFR** Field(RE::InterfaceManager* a_im, int a_which)
		{
			switch (a_which) {
			case settings::kPickRef: return &a_im->pickRef;
			case settings::kReticleRef: return &a_im->reticleRef;
			case settings::kCrosshairRef: return &a_im->crosshairRef;
			case settings::kActivateRef: return &a_im->activateRef;
			default: return nullptr;
			}
		}

		// every field a mode writes (all four for kAllFour)
		std::vector<int> FieldsOf(int a_applyTo)
		{
			if (a_applyTo == settings::kAllFour) {
				return { settings::kPickRef, settings::kReticleRef, settings::kCrosshairRef, settings::kActivateRef };
			}
			if (a_applyTo > settings::kObserve && a_applyTo < settings::kAllFour) {
				return { a_applyTo };
			}
			return {};
		}

		// the game's pick chain, logged on every change (the probe the plan asks for)
		void WatchGameFields(RE::InterfaceManager* a_im, bool a_log)
		{
			struct Seen
			{
				RE::TESObjectREFR* refs[5]{};
				bool               fuzzy = false;
				std::int32_t       pickDistance = 0;
				bool operator==(const Seen&) const = default;
			};
			static Seen last;
			Seen        now;
			now.refs[0] = a_im->pickRef;
			now.refs[1] = a_im->reticleRef;
			now.refs[2] = a_im->crosshairRef;
			now.refs[3] = a_im->activateRef;
			now.refs[4] = a_im->telekinesisRef;
			now.fuzzy = a_im->fuzzyActivatePick;
			now.pickDistance = a_im->pickDistance;
			if (now == last) {
				return;
			}
			last = now;
			static constexpr const char* names[5]{ "pickRef", "reticleRef", "crosshairRef", "activateRef", "telekinesisRef" };
			json fields = json::object();
			std::string line;
			for (int i = 0; i < 5; ++i) {
				const bool ours = now.refs[i] && now.refs[i] == g_written;
				const std::string n = now.refs[i] ? Name(now.refs[i]) + (ours ? " (ours)" : "") : "-";
				fields[names[i]] = n;
				line += std::format("{}{} {}", i ? ", " : "", names[i], n);
			}
			fields["fuzzyActivatePick"] = now.fuzzy;
			fields["pickDistance"] = now.pickDistance;
			fields["activatePickLocation"] = json::array({ a_im->activatePickLocation.x, a_im->activatePickLocation.y, a_im->activatePickLocation.z });
			{
				std::scoped_lock l(g_lock);
				g_gameFields = fields;
			}
			if (a_log) {
				logger::info("game pick: {}; fuzzy {}, pick distance {}", line, now.fuzzy, now.pickDistance);
			}
		}

		void Publish(Status a_status, const std::vector<Candidate>& a_top)
		{
			// where the frame stopped, once per change - an info-level log then shows which stage a round reached
			static int lastReason = -1;
			if (static_cast<int>(a_status.reason) != lastReason) {
				lastReason = static_cast<int>(a_status.reason);
				logger::info("state: {}", a_status.reason == Reason::kActive ? "selecting" : a_status.why);
			}
			std::vector<json> top;
			for (std::size_t i = 0; i < a_top.size() && i < 5; ++i) {
				const auto& c = a_top[i];
				auto*       base = c.ref->data.objectReference;
				top.push_back({ { "ref", Name(c.ref) }, { "type", base ? std::string(RE::FormTypeToString(base->GetFormType())) : "?" },
					{ "angle", std::round(c.angle * 10) / 10 }, { "dist", std::round(c.dist) }, { "score", std::round(c.score * 1000) / 1000 },
					{ "altar_loaded", c.ref->isAltarRefLoaded }, { "oblivion_loaded", c.ref->isOblivionRefLoaded } });
			}
			std::scoped_lock l(g_lock);
			g_status = std::move(a_status);
			g_top = std::move(top);
		}
	}

	void Tick()
	{
		const auto s = settings::Snapshot();
		auto*      im = RE::InterfaceManager::GetInstance(false, false);
		auto*      player = RE::PlayerCharacter::GetSingleton();
		Status     st;
		st.applyTo = settings::ApplyToName(s.applyTo);
		if (!im || !player) {
			st.reason = Reason::kNotReady;
			st.why = "the game is not ready";
			Publish(st, {});
			return;
		}

		// last frame's write: did the game keep it? (logged once per change of the answer)
		if (g_written) {
			static int lastHeld = -1;
			int        held = 1;
			for (const int f : FieldsOf(g_writtenTo)) {
				if (auto** p = Field(im, f); p && *p != g_written) {
					held = 0;
				}
			}
			if (held != lastHeld) {
				lastHeld = held;
				logger::info("write check: {} {} by the next frame", settings::ApplyToName(g_writtenTo), held ? "kept" : "replaced by the game");
			}
		}

		WatchGameFields(im, s.logTargets);

		const auto clearOurs = [&] {
			if (!g_written) {
				return;
			}
			for (const int f : FieldsOf(g_writtenTo)) {
				if (auto** p = Field(im, f); p && *p == g_written) {
					*p = nullptr;   // only what this mod put there; the game's own value is never touched
				}
			}
			g_written = nullptr;
		};

		if (!s.enabled) {
			clearOurs();
			st.reason = Reason::kOff;
			st.why = "switched off";
			Publish(st, {});
			return;
		}
		if (im->menuMode != 1) {   // Oblivion Remastered: 1 is gameplay, anything else a menu (as Tween Menu and Improved Wheel Menu read it)
			st.reason = Reason::kMenu;
			st.why = "a menu is open";
			Publish(st, {});
			return;
		}
		const auto view = camera::Read(player);   // read in either view, so the calibration keeps learning
		// the view, debounced: is3rdPerson reads false for a single frame every few seconds in third person (round 2,
		// 09:49 - "off in first person" for ~13 ms), which would drop the write for that frame; a switch counts once it
		// has held for 150 ms
		static bool      third = player->is3rdPerson;
		static ULONGLONG differentSince = 0;
		if (player->is3rdPerson != third) {
			const ULONGLONG now = GetTickCount64();
			if (!differentSince) {
				differentSince = now;
			} else if (now - differentSince >= 150) {
				third = player->is3rdPerson;
				differentSince = 0;
			}
		} else {
			differentSince = 0;
		}
		if (third ? !s.thirdPerson : !s.firstPerson) {
			clearOurs();
			st.reason = third ? Reason::kOffThird : Reason::kOffFirst;
			st.why = third ? "off in third person" : "off in first person";
			Publish(st, {});
			return;
		}
		if (!view.ok) {
			clearOurs();
			st.reason = camera::Calibrated() ? Reason::kNoCamera : Reason::kCalibrating;
			st.why = camera::Calibrated() ? "the camera cannot be read" : "calibrating - walk a few steps";
			Publish(st, {});
			return;
		}

		// the game's own pick: whichever of its fields holds something this mod did not put there
		RE::TESObjectREFR* game = nullptr;
		for (auto* r : { im->activateRef, im->crosshairRef, im->reticleRef, im->pickRef }) {
			if (r && r != g_written) {
				game = r;
				break;
			}
		}

		const auto candidates = Gather(player, view, s);
		RE::TESObjectREFR* choice = candidates.empty() ? nullptr : candidates.front().ref;
		st.active = true;
		st.reason = Reason::kActive;
		st.candidates = static_cast<std::uint32_t>(candidates.size());
		st.choice = choice ? Name(choice) : "";
		st.game = game ? Name(game) : "";

		static RE::TESObjectREFR* lastChoice = nullptr;
		if (choice != lastChoice) {
			lastChoice = choice;
			if (s.logTargets) {
				if (choice) {
					logger::info("selection: {} ({:.1f} degrees, {:.0f} units, {} candidates){}", Name(choice), candidates.front().angle,
						candidates.front().dist, candidates.size(), game ? std::format(" - the game's own pick {} wins", Name(game)) : "");
				} else {
					logger::info("selection: nothing within {:.0f} units and {:.0f} degrees", s.range, s.maxAngle);
				}
			}
		}

		// write only where the game picked nothing, and only in a test mode until the probe names the field
		if (s.applyTo != settings::kObserve && choice && !game) {
			if (g_written && g_writtenTo != s.applyTo) {
				clearOurs();
			}
			for (const int f : FieldsOf(s.applyTo)) {
				if (auto** p = Field(im, f)) {
					*p = choice;
				}
			}
			g_written = choice;
			g_writtenTo = s.applyTo;
		} else {
			clearOurs();
		}
		Publish(st, candidates);
	}

	Status GetStatus()
	{
		std::scoped_lock l(g_lock);
		return g_status;
	}

	json State()
	{
		std::scoped_lock l(g_lock);
		return { { "active", g_status.active }, { "why", g_status.why }, { "choice", g_status.choice }, { "game", g_status.game },
			{ "candidates", g_status.candidates }, { "apply_to", g_status.applyTo }, { "cells", g_cellsScanned.load() }, { "top", g_top },
			{ "game_fields", g_gameFields } };
	}
}
