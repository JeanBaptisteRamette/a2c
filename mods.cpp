#include "structs.hpp"
#include "utils.hpp"
#include "mods.hpp"

#define CMD_CALLBACK(f) (void(*)())(f)

using namespace a2c::literals;

namespace a2c::Hks
{
    void StubGameConOut(const char* OutputString)
    {
        LogFmtString("GameConOut: {}\n", OutputString);
    }
}


namespace a2c::Scr
{
    void CbSetWinTitle(const char* Title)
    {
        SetWindowTitle(Title);
    }

    void CbEjectMod()
    {
        LogFmtString("Ejecting mod...");
    }

    void CbToggleFly()
    {
        if (LocalPlayer->SpectateMode != 5)
            LocalPlayer->SpectateMode = 5;
        else
            LocalPlayer->SpectateMode = 0;
    }

    void CbDumpCommands(const char* OutputFile)
    {
        if (!OutputFile)
            return;

        // Traversing the hashmap is hard, debug the game first
    }
}

namespace a2c
{
    void SetWindowTitle(std::string_view Title)
    {
        typedef void(*FuncType)(void*, const char*);

        const auto SDL_SetWindowTitle = nt::ResolveAPI<FuncType>(L"SDL2.dll", "SDL_SetWindowTitle");

        if (SDL_SetWindowTitle)
            SDL_SetWindowTitle(*SDL_Window.Pointer(), Title.data());
    }

    void ExtendScriptCommands()
    {
        RegisterConsoleCommand(CMD_CALLBACK(Scr::CbSetWinTitle),  "wintitle", "s");
        RegisterConsoleCommand(CMD_CALLBACK(Scr::CbEjectMod),     "eject", "");
        RegisterConsoleCommand(CMD_CALLBACK(Scr::CbToggleFly),    "fly", "");
        RegisterConsoleCommand(CMD_CALLBACK(Scr::CbDumpCommands), "dumpall", "s");
    }
}