/*
 * compat.h - This header provides the necessary macros and includes for the
 *            given target.
 */

#if defined TERMBUILD
#include <curses.h>
#else  // TERMBUILD
#if defined __linux__
#include <xcurses/curses.h>
#else  // __linux__
#include <curses.h>
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
