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

    LogFmtString("ImageBase:   {}\n", (LPVOID)PebImageBase());
    LogFmtString("PlayerBase:  {}\n", (LPVOID)LocalPlayer.Pointer());

    SetWindowTitle("AC Mod");
    PrintGameConsole("Hello from a2c.dll");
    ExtendScriptCommands();

    Hks::DetourCommandsHashmapFind.Install();

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

