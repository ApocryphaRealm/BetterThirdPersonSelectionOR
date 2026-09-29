#pragma once

// ============================================================================================================
// Just enough Unreal to read the camera (game thread only). CommonLibOB64 has no FProperty, so a property is found
// by NAME by walking a struct's FField chain (next +0x18, name +0x20) and reading FProperty::Offset_Internal at +0x44
// (UE5's layout) - SelfCheck() proves that offset on a property whose place is already known before anything else
// trusts it. Taken from Tween Menu for Oblivion's Reflect/Ue helpers (logic library 7701, 7720), trimmed to what this
// mod reads. Every look-up here is cached by the caller: FName::ToString allocates.
// ============================================================================================================

namespace ue
{
	std::string Utf8(const UE::FString& a_s);
	std::string NameOf(UE::UObject* a_o);

	// -1 when the struct (or its supers) has no property of that name
	std::int32_t Offset(UE::UStruct* a_struct, std::string_view a_name);

	bool SelfCheck();   // true once the Offset_Internal layout is proven
	bool IsLive(UE::UObject* a_o);

	// the first live object whose class is a_base or derives from it (not a class default object) - scans the whole
	// object array, so the caller caches the answer
	UE::UObject* FirstOf(UE::UClass* a_base);

	inline UE::UClass* Class(const wchar_t* a_path)
	{
		return UE::StaticFindObject<UE::UClass>(nullptr, nullptr, a_path);
	}

	template <class T>
	T* At(void* a_base, std::int32_t a_offset)
	{
		return a_base && a_offset >= 0 ? reinterpret_cast<T*>(static_cast<std::uint8_t*>(a_base) + a_offset) : nullptr;
	}

	// A reflected call with no parameters in and one ReturnValue out, laid out from the UFunction's own properties.
	// The UFunction and its ReturnValue offset are looked up once per (class, name) by the caller's static.
	class Getter
	{
	public:
		Getter(const wchar_t* a_function) :
			m_name(a_function)
		{}

		// false when the object has no such function; a_out gets sizeof(T) bytes from ReturnValue
		template <class T>
		bool Get(UE::UObject* a_obj, T& a_out)
		{
			if (!a_obj || !Resolve(a_obj)) {
				return false;
			}
			m_params.assign(m_params.size(), 0);
			a_obj->ProcessEvent(m_fn, m_params.data());
			std::memcpy(&a_out, m_params.data() + m_ret, sizeof(T));
			return true;
		}

	private:
		bool Resolve(UE::UObject* a_obj);

		const wchar_t*            m_name;
		UE::UClass*               m_class = nullptr;
		UE::UFunction*            m_fn = nullptr;
		std::int32_t              m_ret = -1;
		std::vector<std::uint8_t> m_params;
	};
}
