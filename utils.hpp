#ifndef A2C_UTILS_HPP
#define A2C_UTILS_HPP

#include <string_view>
#include <iostream>
#include <format>

#include <windows.h>


namespace a2c
{
    template<typename ...Args>
    inline void LogFmtString(std::string_view fmt, Args&&... args)
    {
        std::cout << std::vformat(fmt, std::make_format_args(args...));
    }

    template<typename ...Args>
    inline void LogErrFmtString(std::string_view fmt, Args&&... args)
    {
        std::cerr << std::vformat(fmt, std::make_format_args(args...));
    }

    namespace nt
    {
        template<typename ProcType>
        ProcType ResolveAPI(std::wstring_view ModuleName, std::string_view ProcedureName)
        {
            HMODULE Module = GetModuleHandleW(ModuleName.data());

            if (!Module)
                return nullptr;

            FARPROC Procedure = GetProcAddress(Module, ProcedureName.data());
            return reinterpret_cast<ProcType>(Procedure);
        }
    }
}

#endif //A2C_UTILS_HPP
