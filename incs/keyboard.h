#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <windows.h>

BOOL keyboard_translate(
    DWORD vk_code,
    WPARAM message,
    char *buffer,
    int size
);

#endif