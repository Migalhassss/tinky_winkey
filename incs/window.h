#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

BOOL window_changed(void);

void window_print_foreground(void);

void window_get_foreground_process(
    char *process,
    int size
);

#endif