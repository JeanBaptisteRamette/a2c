#include <windows.h>
#include "structs.hpp"
#include "console.hpp"
#include "memory.hpp"
#include "utils.hpp"

using namespace std::chrono_literals;


DWORD WINAPI ThreadEntry(HMODULE HandleModule)
{
    if (!a2c::ConsoleCreate())
        return 1;

    const auto ImageBase  = a2c::PebImageBase();
    const auto PlayerAddr = *(void**)(ImageBase + a2c::LOCAL_PLAYER_PTR);

    auto* Player = reinterpret_cast<a2c::LOCAL_PLAYER*>(PlayerAddr);

    a2c::LogFmtString("ImageBase:  {}\n", (void*)ImageBase);
    a2c::LogFmtString("PlayerAddr: {}\n", (void*)PlayerAddr);

    while (true)
    {
        if (GetAsyncKeyState(VK_DELETE) & 1)
            break;

        Player->Health = 50;

        std::this_thread::sleep_for(500ms);
    }

    a2c::ConsoleDestroy();

    // Unload DLL and stops the thread
    FreeLibraryAndExitThread(HandleModule, 0);

    return 0;
}


BOOL WINAPI DllMain(HINSTANCE ModuleHandle, DWORD Reason, LPVOID Reserved)
{
    if (Reason == DLL_PROCESS_ATTACH)
    {
        HANDLE Thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ThreadEntry), (LPVOID)ModuleHandle, 0, nullptr);

        // Disable DLL_THREAD_ATTACH/DETACH notifications
        // DisableThreadLibraryCalls(ModuleHandle);

        CloseHandle(Thread);
    }

    return TRUE;
}

