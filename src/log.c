#include "compat.h"
#include "log.h"
#include <inttypes.h>
#include <stdlib.h>

static WINDOW* log_window;
static PANEL* log_panel;

static char* text[100] = {0};

void log_init() {
    log_window = newwin(10, 80, 20, 0);
    log_panel = new_panel(log_window);
}

void log_shutdown() {
    for(int i = 0; i < 100; ++i)
        if(text[i] != NULL)
            free(text[i]);
    del_panel(log_panel);
    delwin(log_window);
}

void log_draw() {
    wclear(log_window);
    box(log_window, 0, 0);

    for(int i = 0; i < 8; ++i)
        mvwaddstr(log_window, 8 - i, 1, text[i]);
}

void log_write(const char* fmt, ...) {
    if(text[99] != NULL)
        free(text[99]);
    for(int i = 99; i > 0; --i)
        text[i] = text[i - 1];

    va_list args;
    va_start(args, fmt);
    uint16_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    va_start(args, fmt);
    text[0] = calloc(len + 1, sizeof(char));
    vsnprintf(text[0], len + 1, fmt, args);
    va_end(args);
}
