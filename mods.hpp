#ifndef A2C_MODS_HPP
#define A2C_MODS_HPP


#include <string_view>
#include "detour.hpp"
#include "memory.hpp"


namespace a2c
{
    namespace Hks
    {
        COMMAND_STRUCT* __stdcall CommandsHashmapFind(const char** Key);

        inline Detour DetourCommandsHashmapFind(a2c::CommandHashmapFind, (UPTR)CommandsHashmapFind);
    }

    void SetWindowTitle(std::string_view Title);

    void ExtendScriptCommands();
}

#endif //A2C_MODS_HPP
