#ifndef A2C_STRUCTS_HPP
#define A2C_STRUCTS_HPP

#include <string_view>
#include <cstdint>
#include <format>

#include "memory.hpp"
#include "symbol.hpp"


namespace a2c
{
    struct LOCAL_PLAYER
    {
        char pad_0000[28]; //0x0000
        float AccelX; //0x001C
        float AccelY; //0x0020
        float AccelZ; //0x0024
        float PosX; //0x0028
        float PosY; //0x002C
        float PosZ; //0x0030
        float ViewYaw; //0x0034
        float ViewPitch; //0x0038
        float ViewRoll; //0x003C
        char pad_0040[48]; //0x0040
        int16_t N0000006B; //0x0070
        int16_t TimeInAir; //0x0072
        int16_t unknown; //0x0074
        int16_t State; //0x0076
        char pad_0078[116]; //0x0078
        int32_t Health; //0x00EC
        int32_t Armor; //0x00F0
        char pad_00F4[548]; //0x00F4
        int32_t SpectateMode; //0x0318
        char pad_031C[296]; //0x031C
    }; //Size: 0x0444

    struct COMMAND_STRUCT
    {
        UINT32 a0;
        const char* Name;
        UINT32 a2;
        UINT32 a3;
        UINT32 a4;
        void(*Callback)();
        UINT32 Signature;
    };

    using namespace a2c::literals;

    // Variables
    inline Symbol<LOCAL_PLAYER> LocalPlayer(*(UPTR*)0x18AC00_r);
    const Symbol<void*> SDL_Window(0x585870);
    const Symbol<void*> CommandHashmap(0x583828);

    // Functions
    const Symbol<COMMAND_STRUCT*(const char** Key)> CommandHashmapFind(0x26800_r);
    const Symbol<void(void(*Callback)(), const char* Name, const char* Signature)> RegisterConsoleCommand(0xD69C0_r);
    const Symbol<void(const char* Fmt)> GameConOut(0xDAD50_r);

    template<typename ...Args>
    inline void PrintGameConsole(std::string_view fmt, Args&&... args)
    {
        const auto out = std::vformat(fmt, std::make_format_args(args...));
        GameConOut(out.c_str());
    }
}


#endif //A2C_STRUCTS_HPP
