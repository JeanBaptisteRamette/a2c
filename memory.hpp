#ifndef A2C_MEMORY_HPP
#define A2C_MEMORY_HPP

#include <windows.h>
#include <cstdint>

#define RELOCATE(RVA) (uintptr_t)(PebImageBase() + (RVA))

namespace a2c
{
    inline LPBYTE PebImageBase()
    {
        return reinterpret_cast<LPBYTE>(GetModuleHandleW(nullptr));
    }

    namespace literals
    {
        inline uintptr_t operator ""_r(unsigned long long int Rva)
        {
            return RELOCATE(Rva);
        }
    }
}



#endif //A2C_MEMORY_HPP
