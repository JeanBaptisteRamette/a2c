#include "structs.hpp"
#include "symbol.hpp"
#include "utils.hpp"
#include "mods.hpp"


namespace a2c
{
    void SetWindowTitle(std::string_view Title)
    {
        typedef void(*FuncType)(void*, const char*);

        const auto SDL_SetWindowTitle = nt::ResolveAPI<FuncType>(L"SDL2.dll", "SDL_SetWindowTitle");

        if (SDL_SetWindowTitle)
            SDL_SetWindowTitle(*SDL_Window.Pointer(), Title.data());
    }
}