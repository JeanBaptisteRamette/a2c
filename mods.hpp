#ifndef A2C_MODS_HPP
#define A2C_MODS_HPP


#include <string_view>


namespace a2c
{
    namespace Hks
    {
        void StubGameConOut(const char* OutputString);
    }

    void SetWindowTitle(std::string_view Title);

    void ExtendScriptCommands();
}

#endif //A2C_MODS_HPP
