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
#include <string.h>
#include "window.h"


static HWND last_hwnd = NULL;
static DWORD last_pid = 0;


static void get_foreground_info(
    HWND *hwnd,
    DWORD *pid,
    char *title
)
{
    *hwnd = GetForegroundWindow();

    if (*hwnd == NULL)
    {
        *pid = 0;
        title[0] = '\0';
        return;
    }

    GetWindowTextA(
        *hwnd,
        title,
        256
    );

    GetWindowThreadProcessId(
        *hwnd,
        pid
    );
}


BOOL window_changed(void)
{
    HWND hwnd;
    DWORD pid;
    char title[256];

    get_foreground_info(
        &hwnd,
        &pid,
        title
    );

    if (hwnd != last_hwnd ||
        pid != last_pid)
    {
        last_hwnd = hwnd;
        last_pid = pid;

        return TRUE;
    }

    return FALSE;
}


void window_get_foreground_process(
    char *process,
    int size
)
{
    HWND hwnd;
    DWORD pid;
    HANDLE handle;
    DWORD length;
    char *last;


    if (process == NULL || size <= 0)
        return;


    process[0] = '\0';

    hwnd = GetForegroundWindow();

    if (hwnd == NULL)
        return;


    GetWindowThreadProcessId(
        hwnd,
        &pid
    );


    handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION,
        FALSE,
        pid
    );


    if (handle == NULL)
        return;


    length = size;

    QueryFullProcessImageNameA(
        handle,
        0,
        process,
        &length
    );


    CloseHandle(handle);


    last = strrchr(
        process,
        '\\'
    );


    if (last != NULL)
    {
        memmove(
            process,
            last + 1,
            strlen(last + 1) + 1
        );
    }
}


void window_print_foreground(void)
{
    HWND hwnd;
    DWORD pid;
    char title[256];
    char process[260];


    get_foreground_info(
        &hwnd,
        &pid,
        title
    );


    window_get_foreground_process(
        process,
        sizeof(process)
    );


    printf(
        "Window: %s | Process: %s | PID: %lu\n",
        title,
        process,
        pid
    );
}