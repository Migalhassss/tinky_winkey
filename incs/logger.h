#ifndef LOGGER_H
#define LOGGER_H

#include <windows.h>

BOOL logger_init(void);

void logger_close(void);

void logger_write_window(void);

void logger_write_key(
    const char *key
);

#endif