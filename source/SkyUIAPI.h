#pragma once
#include "ModuleList.hpp"

struct ApiCALL {
	static inline HMODULE h = nullptr;

	static void* GetFunctionByName(const char* name) {
		if (!h)
			h = ModuleList().GetByPrefix(L"skyui");

		if (h) {
			auto a = (void* (*)())GetProcAddress(h, name);

			if (a) {
				return a;
			}
		}
		return NULL;
	}

	template <typename... Args>
	static void Call(const char* name, Args... args) {
		void* f = GetFunctionByName(name);
		if (f)
			reinterpret_cast<void(__cdecl*)(Args...)>(f)(args...);
	}

	template <typename Ret, typename... Args>
	static Ret CallAndReturn(const char* name, Args... args) {
		void* f = GetFunctionByName(name);

		if (f)
			return reinterpret_cast<Ret(__cdecl*)(Args...)>(f)(args...);

		return NULL;
	}

	template <typename... Args>
	static void CallMethod(const char* name, Args... args) {
		void* f = GetFunctionByName(name);
		if (f)
			reinterpret_cast<void(__thiscall*)(Args...)>(f)(args...);
	}

	template <typename Ret, typename... Args>
	static Ret CallMethodAndReturn(const char* name, Args... args) {
		void* f = GetFunctionByName(name);

		if (f)
			return reinterpret_cast<Ret(__thiscall*)(Args...)>(f)(args...);

		return NULL;
	}
};

class CMenuManager;

class SkyUI {
public:
	static inline uint32_t GetAlpha(uint32_t a = 255) {
		return ApiCALL::CallAndReturn<uint32_t>(__FUNCTION__, a);
	}

	static inline float GetMenuOffsetX() {
		return ApiCALL::CallAndReturn<float>(__FUNCTION__);
	}

	static inline bool GetGTA3LCS() {
		return ApiCALL::CallAndReturn<bool>(__FUNCTION__);
	}

	static inline uint8_t GetCurrentInput() {
		return ApiCALL::CallAndReturn<uint8_t>(__FUNCTION__);
	}

	static inline int32_t GetTimeToWaitBeforeStateChange() {
		return ApiCALL::CallAndReturn<int32_t>(__FUNCTION__);
	}

	static inline uint8_t GetCheckHoverForStandardInput(CMenuManager* _this) {
		return ApiCALL::CallAndReturn<uint8_t, CMenuManager*>(__FUNCTION__, _this);
	}

	typedef uint8_t(*MenuOptionCB)(uint32_t action, int8_t arrows, bool* back, bool enter);
	
#if defined(GTASA)
	using char_t = char;
#else
	using char_t = wchar_t;
#endif
	typedef char_t* (*MenuOptionStringsCB)(uint32_t action);

	static inline void ProcessMenuOptionsCB(MenuOptionCB cb) {
		ApiCALL::Call(__FUNCTION__, cb);
	}

	static inline void ProcessMenuOptionsStringsCB(MenuOptionStringsCB cb) {
		ApiCALL::Call(__FUNCTION__, cb);
	}

	static inline void AddEntryToMenuScreen(uint32_t screen, uint32_t entry, uint32_t action, const char* entryName, uint32_t targetScreen, uint32_t orientation) {
		ApiCALL::Call(__FUNCTION__, screen, entry, action, entryName, targetScreen, orientation);
	}

	static inline void SaveOrLoadSettingsCB(void (*cb)(bool isLoading)) {
		ApiCALL::Call(__FUNCTION__, cb);
	}

	static inline void SaveSettings() {
		ApiCALL::Call(__FUNCTION__);
	}
};
