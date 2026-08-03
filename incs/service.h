#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // Define Windows 10/11

#pragma warning(push)
#pragma warning(disable: 4668) // Desativa avisos de macros do Windows SDK
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <shellapi.h>
#pragma warning(pop)

int install_service(void);
int delete_service(void);
int start_service(void);
int stop_service(void);

#endif