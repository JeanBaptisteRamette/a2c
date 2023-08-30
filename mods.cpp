#include "structs.hpp"
#include "utils.hpp"
#include "mods.hpp"


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
        RegisterConsoleCommand((void (*)())Scr::CbSetWinTitle, "wintitle", "s");
        RegisterConsoleCommand(Scr::CbEjectMod, "eject", "");
        RegisterConsoleCommand(Scr::CbToggleFly, "fly", "");
    }
}