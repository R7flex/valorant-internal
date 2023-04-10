#include "valorant.hpp"

bool DllMain(void*, uint32_t call_reason, void*)
{
	if (call_reason == DLL_PROCESS_ATTACH) {
		AllocConsole();
		FILE* fp;
		freopen_s(&fp, ("CONOUT$"), ("w"), stdout);
		std::uintptr_t image_base = reinterpret_cast<std::uintptr_t>( GetModuleHandleA(0) );
		valorant::start::init_cheat(image_base);
	}

	return true;
}