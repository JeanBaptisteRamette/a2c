#ifndef A2C_DETOUR_HPP
#define A2C_DETOUR_HPP

#include <array>
#include <cstdint>
#include <string_view>


namespace a2c
{
#pragma pack(push, 1)

    //
    // 32-bit relative unconditional jump
    //

    struct X86_JMP_REL
    {
        uint8_t Opcode;
        uint32_t RelativeAddress;
    };
#pragma pack(pop)

    // 3 words to save 5 overwritten bytes
    constexpr auto PATCH_WORDS = sizeof(X86_JMP_REL) / sizeof(uint16_t) + 1;


    class Detour
    {
    public:
        Detour(uintptr_t Target, uintptr_t Detour);
        Detour(std::string_view ModuleName, std::string_view ProcName, uintptr_t Detour);

        ~Detour();

        void Install();
        void Uninstall();

        [[nodiscard]] bool Installed() const;

    private:
        uintptr_t TargetAddr;
        uintptr_t DetourAddr;

        bool Enabled {};

        std::array<uint16_t, PATCH_WORDS> StolenData {};
    };
}

#endif //A2C_DETOUR_HPP
