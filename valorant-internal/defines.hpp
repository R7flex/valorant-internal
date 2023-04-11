#include <Windows.h>
#include <iostream>
#include <cstdio>

namespace valorant
{
	inline std::uintptr_t image_base = 0x0;
}

namespace offsets
{
	//function
	constexpr std::uintptr_t process_event = 0x337B180;
	constexpr std::uintptr_t static_find_object = 0x338F740;
	constexpr std::uintptr_t bone_matrix = 0x4A54980;
	constexpr std::uintptr_t trigger_veh = 0x1A8FDB0;

	//offsets
	constexpr std::uintptr_t game_instance = 0x1a0;
	constexpr std::uintptr_t local_players = 0x40;
	constexpr std::uintptr_t viewportclient = 0x78;
	constexpr std::uintptr_t world = 0x80;
}

namespace memory
{
	template <typename T>
	T read(std::uintptr_t addr)
	{
		T buffer;
		ReadProcessMemory(GetCurrentProcess(), (LPCVOID)addr, &buffer, sizeof(T), 0);
		return buffer;
	}
}

namespace crt
{
	__forceinline int wcslen(wchar_t* str) {
		int counter = 0;
		if (!str)
			return 0;
		for (; *str != '\0'; ++str)
			++counter;
		return counter;
	}
}