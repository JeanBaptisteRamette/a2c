#include <windows.h>
#include "structs.hpp"
#include "console.hpp"
#include "memory.hpp"
#include "detour.hpp"
#include "utils.hpp"
#include "mods.hpp"


using namespace std::chrono_literals;
using namespace a2c::literals;


a2c::Detour DetourGameConOut(0xDAD50_r, (uintptr_t)a2c::Hks::StubGameConOut);


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

    DetourGameConOut.Install();

    while (true)
    {
        if (GetAsyncKeyState(VK_DELETE) & 1)
            break;


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

        CloseHandle(Thread);
    }

    return TRUE;
}

