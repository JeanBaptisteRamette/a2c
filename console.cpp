#include <cstdio>
#include <windows.h>
#include "console.hpp"


namespace a2c
{
    static FILE* StreamOut = nullptr;
    // static FILE* StreamErr = nullptr;

    bool ConsoleCreate()
    {
        if (!AllocConsole())
            return false;

        // Redirect output streams to the console
        const errno_t ec1 = freopen_s(&StreamOut, "CONOUT$", "w", stdout);
        // const errno_t ec2 = freopen_s(&StreamErr, "CONERR$", "w", stderr);

        if (ec1 /* || ec2 */)
            return false;

        return true;
    }

    void ConsoleDestroy()
    {
        if (StreamOut) fclose(StreamOut);
        // if (StreamErr) fclose(StreamErr);

        FreeConsole();
    }
}