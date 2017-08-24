#ifndef LOG_H
#define LOG_H

void log_init();
void log_shutdown();

void log_draw();
void log_write(const char* fmt, ...);

#endif
