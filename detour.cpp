#include "detour.hpp"
#include <windows.h>
#include <stdexcept>


namespace a2c
{
    void CreateRedirectStub(uintptr_t TargetAddr, uintptr_t DetourAddr)
    {
        auto Stub = reinterpret_cast<X86_JMP_REL*>(TargetAddr);

        Stub->Opcode = 0xE9;
        Stub->RelativeAddress = DetourAddr - (TargetAddr + sizeof(X86_JMP_REL));
    }

    void EnableRedirect(uintptr_t TargetAddr, uintptr_t DetourAddr, bool Enable, std::array<uint16_t, PATCH_WORDS>& StolenData)
    {
        DWORD ProtectionFlags;
        VirtualProtect((LPVOID)TargetAddr, sizeof(X86_JMP_REL), PAGE_EXECUTE_READWRITE, &ProtectionFlags);

        if (Enable)
        {
            auto* Tmp = reinterpret_cast<uint16_t*>(TargetAddr);

            // Save the 5 overwritten bytes
            StolenData[0] = Tmp[0];
            StolenData[1] = Tmp[1];
            StolenData[2] = Tmp[2];

            CreateRedirectStub(TargetAddr, DetourAddr);
        }
        else
        {
            auto* Tmp = reinterpret_cast<uint16_t*>(TargetAddr);

            Tmp[0] = StolenData[0];
            Tmp[1] = StolenData[1];
            Tmp[2] = StolenData[2];
        }

        VirtualProtect((LPVOID)TargetAddr, sizeof(X86_JMP_REL), ProtectionFlags, &ProtectionFlags);

        // The CPU cannot detect the change in memory, and may execute the old code it cached.
        // Prevent this with FlushInstructionCache.
        FlushInstructionCache(GetCurrentProcess(), (LPVOID)TargetAddr, sizeof(X86_JMP_REL));
    }


    Detour::Detour(uintptr_t Target, uintptr_t Detour) : TargetAddr(Target), DetourAddr(Detour) {}

    Detour::Detour(std::string_view ModuleName, std::string_view ProcName, uintptr_t Detour) : DetourAddr(Detour)
    {
        const HMODULE Module = GetModuleHandleA(ModuleName.data());

        if (!Module)
        {
            using namespace std::string_literals;
            throw std::runtime_error("GetModuleHandle("s + ModuleName.data() + ") failed");
        }

        const auto Target = GetProcAddress(Module, ProcName.data());

        if (!Target)
        {
            using namespace std::string_literals;
            throw std::runtime_error("GetProcAddress("s + ProcName.data() + ") failed");
        }

        TargetAddr = (uintptr_t)Target;
    }

    Detour::~Detour()
    {
        if (Enabled)
            Uninstall();
    }

    void Detour::Install()
    {
        if (!TargetAddr || !DetourAddr || DetourAddr == TargetAddr)
            return;

        if (Enabled)
            return;

        EnableRedirect(TargetAddr, DetourAddr, true, StolenData);

        Enabled = true;
    }

    void Detour::Uninstall()
    {
        if (!Enabled)
            return;

        EnableRedirect(TargetAddr, DetourAddr, false, StolenData);

        Enabled = false;
    }

    bool Detour::Installed() const
    {
        return Enabled;
    }
}