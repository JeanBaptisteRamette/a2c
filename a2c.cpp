#include <windows.h>
#include "structs.hpp"
#include "console.hpp"
#include "memory.hpp"
#include "utils.hpp"
#include "mods.hpp"

using namespace std::chrono_literals;
using namespace a2c::literals;


DWORD WINAPI ThreadEntry(HMODULE HandleModule)
{
    using namespace a2c;

    if (!ConsoleCreate())
        return 1;

    LogFmtString("ImageBase:   {}\n", (void*)PebImageBase());
    LogFmtString("PlayerBase:  {}\n", (void*)LocalPlayer.Pointer());

    SetWindowTitle("AC Mod");
    PrintGameConsole("Hello from a2c.dll");
    ExtendScriptCommands();


    while (true)
    {
        if (GetAsyncKeyState(VK_DELETE) & 1)
            break;

        LocalPlayer->Health = 50;

        std::this_thread::sleep_for(500ms);
    }

    ConsoleDestroy();

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

