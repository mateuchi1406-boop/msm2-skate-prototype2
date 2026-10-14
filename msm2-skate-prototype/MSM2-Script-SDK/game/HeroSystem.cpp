#include "HeroSystem.h"

#include <string>
#include "../utils.h"

HeroSystem* GetHeroSystem() {
    // position-dependent =\

    const std::string module_name = utils::get_game_executable();
    const HMODULE handle = GetModuleHandleA(module_name.c_str());
    const uintptr_t base = (uintptr_t)handle;
    return (HeroSystem*)(base + 0x9BD1120); // 2.727.0.0
}
