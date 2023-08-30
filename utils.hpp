#ifndef A2C_UTILS_HPP
#define A2C_UTILS_HPP

#include <string_view>
#include <iostream>
#include <format>


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
}

#endif //A2C_UTILS_HPP
