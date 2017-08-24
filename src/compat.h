/*
 * compat.h - This header provides the necessary macros and includes for the
 *            given target.
 */

#if defined TERMBUILD
#include <curses.h>
#include <panel.h>
#else  // TERMBUILD
#if defined __linux__
#include <xcurses/curses.h>
#include <xcurses/panel.h>
#else  // __linux__
#include <curses.h>
#include <panel.h>
#endif // __linux__
#endif // TERMBUILD

// Windows and POSIX use different timing functions, so we use this macro to
// determine the right one to use
#if defined _WIN32
#include <windows.h>
#define msleep(x) Sleep(x)
#else  // _WIN32
#include <unistd.h>
#define msleep(x) usleep(x * 1000)
#endif // _WIN32

#define VIEW_LINES 25
#define VIEW_COLS 80
