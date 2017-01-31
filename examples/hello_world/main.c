// Include the correct header for the library that we're using
#if defined PDCURSES
/* #include <xcurses/curses.h> */
#include <curses.h>
#else
#include <curses.h>
#endif

int main() {
#if defined PDCURSES
    putenv("PDC_COLS=120");
    putenv("PDC_LINES=40");
    resize_term(40, 120);
#endif

    // Initialize curses
    initscr();

    printw("Hello, World!");
    move(1, 0);
    printw("Press any key to continue...");

    // Pause the program until we press a key
    getch();

    // Clean up
    endwin();

    return 0;
}
