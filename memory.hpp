#ifndef A2C_MEMORY_HPP
#define A2C_MEMORY_HPP

#include <windows.h>
#include <cstdint>

namespace a2c
{
    enum OffsetType
    {
        LOCAL_PLAYER_PTR = 0x18AC00
    };

    inline LPBYTE PebImageBase()
    {
        return reinterpret_cast<LPBYTE>(GetModuleHandleW(nullptr));
    }
}



#endif //A2C_MEMORY_HPP
