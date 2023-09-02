#ifndef A2C_DETOUR_HPP
#define A2C_DETOUR_HPP

#include <array>
#include <cstdint>
#include <string_view>
#include "mindef.hpp"
#include "symbol.hpp"


namespace a2c
{
#pragma pack(push, 1)

    //
    // 32-bit relative unconditional jump
    //

    struct X86_JMP_REL
    {
        UINT8  Opcode;
        UINT32 RelativeAddress;
    };
#pragma pack(pop)


    inline void CreateRedirectStub(UPTR TargetAddr, UPTR DetourAddr)
    {
        volatile X86_JMP_REL* Stub = reinterpret_cast<X86_JMP_REL*>(TargetAddr);

        Stub->Opcode = 0xE9;
        Stub->RelativeAddress = DetourAddr - (TargetAddr + sizeof(X86_JMP_REL));
    }

    template<UINT SavedBytesCount>
    void EnableRedirect(UPTR TargetAddr, UPTR DetourAddr, bool Enable, std::array<BYTE, SavedBytesCount>& StolenData)
    {
        DWORD ProtectionFlags;
        VirtualProtect((LPVOID)TargetAddr, SavedBytesCount, PAGE_EXECUTE_READWRITE, &ProtectionFlags);

        if (Enable)
        {
            // Save
            std::memcpy(StolenData.data(), reinterpret_cast<LPVOID>(TargetAddr), SavedBytesCount);
            CreateRedirectStub(TargetAddr, DetourAddr);
        }
        else
        {
            // Restore
            std::memcpy(reinterpret_cast<LPVOID>(TargetAddr), StolenData.data(), SavedBytesCount);
        }

        VirtualProtect((LPVOID)TargetAddr, SavedBytesCount, ProtectionFlags, &ProtectionFlags);

        // The CPU cannot detect the change in memory, and may execute the old code it cached.
        // Prevent this with FlushInstructionCache.
        FlushInstructionCache(GetCurrentProcess(), (LPVOID)TargetAddr, sizeof(X86_JMP_REL));
    }

    // Returns a pointer to a dynamically allocated block
    // which contains a stub with the original bytes of the target
    // then a jump to the original target past the patched bytes
    inline LPVOID CreateTrampoline(UPTR Target, SIZE_T SavedBytesCount)
    {
        SIZE_T AllocationSize = SavedBytesCount + sizeof(X86_JMP_REL);

        LPVOID Trampoline = VirtualAlloc(nullptr, AllocationSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

        if (!Trampoline)
            return nullptr;

        // Read SavedBytesCount at address Target
        std::memcpy(Trampoline, (LPVOID)Target, SavedBytesCount);

        // Add jmp to original function + patched bytes
        CreateRedirectStub((UPTR)Trampoline + SavedBytesCount, Target + SavedBytesCount);

        return Trampoline;
    }


    template<SIZE_T SavedBytesCount = 5>
    class Detour
    {
        // SIZE_T SavedBytesCount: If we only save 5 bytes, it may not save a whole instruction,
        //                         so let the caller specify the amount of bytes to save

    public:
        template<typename FunctionSig>
        Detour(const Symbol<FunctionSig>& GameFunc, UPTR Detour) : TargetAddr(GameFunc.Address()), DetourAddr(Detour)
        {
            Trampoline = CreateTrampoline(TargetAddr, SavedBytesCount);
        }

        Detour(UPTR Target, UPTR Detour) : TargetAddr(Target), DetourAddr(Detour)
        {
            Trampoline = CreateTrampoline(TargetAddr, SavedBytesCount);
        }


        Detour(std::string_view ModuleName, std::string_view ProcName, UPTR Detour) : DetourAddr(Detour)
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

            TargetAddr = (UPTR)Target;

            Trampoline = CreateTrampoline(TargetAddr, SavedBytesCount);
        }

        ~Detour()
        {
            if (Enabled)
                Uninstall();

            if (Trampoline)
                VirtualFree(Trampoline, 0, MEM_RELEASE);
        }

        void Install()
        {
            if (!TargetAddr || !DetourAddr || DetourAddr == TargetAddr)
                return;

            if (Enabled)
                return;

            EnableRedirect(TargetAddr, DetourAddr, true, StolenData);

            Enabled = true;
        }

        void Uninstall()
        {
            if (!Enabled)
                return;

            EnableRedirect(TargetAddr, DetourAddr, false, StolenData);

            Enabled = false;
        }

        [[nodiscard]] bool Installed() const
        {
            return Enabled;
        }

        template<typename T = void, typename ...Args>
        T Invoke(Args ...args)
        {
            typedef T(*Function)(Args...args);

            // Call the trampoline to the original function
            return reinterpret_cast<Function>(Trampoline)(args...);
        }

    private:
        UPTR TargetAddr;
        UPTR DetourAddr;

        LPVOID Trampoline {};

        bool Enabled {};

        std::array<BYTE, SavedBytesCount> StolenData {};
    };
}

#endif //A2C_DETOUR_HPP
