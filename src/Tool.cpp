// selection.state (rules 31 and 64): op state (default) - the settings, what this mod and the game picked, the best
// few candidates, the game's pick fields, the camera and its calibration; op set {key, value} - change one setting as
// the page would (saved at once): enabled, thirdPerson, firstPerson, range, maxAngle, applyTo (0-5), logTargets;
// op reset - the defaults. Every accessor used here is thread-safe, so the handler answers on TestBench's own thread.
#include "Camera.h"
#include "Selection.h"
#include "Settings.h"
#include "TestBenchAPI.h"

namespace tool
{
	namespace
	{
		TestBenchAPI::ITestBenchInterface001* g_tb = nullptr;

		void Write(void* a_sink, TestBenchAPI::WriteFn a_write, const json& a_j) { a_write(a_sink, a_j.dump().c_str()); }

		json SettingsJson()
		{
			const auto v = settings::Snapshot();
			return { { "enabled", v.enabled }, { "thirdPerson", v.thirdPerson }, { "firstPerson", v.firstPerson }, { "range", v.range },
				{ "maxAngle", v.maxAngle }, { "applyTo", v.applyTo }, { "applyToName", settings::ApplyToName(v.applyTo) },
				{ "logTargets", v.logTargets } };
		}

		// false when a_key is not a setting or a_value has the wrong type
		bool Set(const std::string& a_key, const json& a_value)
		{
			const bool isBool = a_value.is_boolean(), isNum = a_value.is_number();
			if ((a_key == "enabled" || a_key == "thirdPerson" || a_key == "firstPerson" || a_key == "logTargets") && isBool) {
				const bool b = a_value.get<bool>();
				settings::Update([&](settings::Values& s) {
					(a_key == "enabled" ? s.enabled : a_key == "thirdPerson" ? s.thirdPerson : a_key == "firstPerson" ? s.firstPerson : s.logTargets) = b;
				});
			} else if ((a_key == "range" || a_key == "maxAngle") && isNum) {
				const float f = a_value.get<float>();
				settings::Update([&](settings::Values& s) { (a_key == "range" ? s.range : s.maxAngle) = f; });
			} else if (a_key == "applyTo" && isNum) {
				const int i = a_value.get<int>();
				settings::Update([&](settings::Values& s) { s.applyTo = i; });
			} else {
				return false;
			}
			logger::info("tool: {} set to {}", a_key, a_value.dump());
			return true;
		}

		void Tool(void*, const char* a_args, void* a_sink, TestBenchAPI::WriteFn a_write)
		{
			json args = json::parse(a_args ? a_args : "{}", nullptr, false);
			if (args.is_discarded() || !args.is_object()) args = json::object();
			const std::string op = args.value("op", "state");
			if (op == "set") {
				const std::string key = args.value("key", std::string());
				if (!args.contains("value") || !Set(key, args["value"])) {
					Write(a_sink, a_write, { { "ok", false }, { "error", "set {key, value}: enabled|thirdPerson|firstPerson|logTargets (bool), range|maxAngle (number), applyTo (0 observe, 1 pickRef, 2 reticleRef, 3 crosshairRef, 4 activateRef, 5 all four)" } });
					return;
				}
				Write(a_sink, a_write, { { "ok", true }, { "settings", SettingsJson() } });
				return;
			}
			if (op == "reset") {
				settings::Reset();
				Write(a_sink, a_write, { { "ok", true }, { "settings", SettingsJson() } });
				return;
			}
			if (op != "state") {
				Write(a_sink, a_write, { { "ok", false }, { "error", "op: state | set {key, value} | reset" } });
				return;
			}
			Write(a_sink, a_write, { { "ok", true }, { "version", SEL_VERSION }, { "settings", SettingsJson() }, { "selection", selection::State() },
				{ "camera", camera::State() } });
		}
	}

	bool Register()
	{
		if (g_tb) return true;
		HMODULE tb = ::GetModuleHandleW(L"TestBench.dll");
		auto get = tb ? reinterpret_cast<void* (*)(unsigned)>(::GetProcAddress(tb, "TestBench_GetInterface")) : nullptr;
		g_tb = get ? static_cast<TestBenchAPI::ITestBenchInterface001*>(get(1)) : nullptr;
		if (!g_tb) return false;
		g_tb->RegisterTool("selection.state",
			R"({"description":"Better Selection: op state (default) - settings, this mod's pick and the game's, the best candidates, the game's pick fields (pickRef/reticleRef/crosshairRef/activateRef/fuzzyActivatePick), the camera and its calibration; op set {key, value} - enabled, thirdPerson, firstPerson, range, maxAngle, applyTo 0-5, logTargets; op reset","inputSchema":{"type":"object","properties":{"op":{"type":"string"},"key":{"type":"string"},"value":{}}}})",
			&Tool, nullptr);
		logger::info("TestBench tool registered: selection.state");
		return true;
	}
}
