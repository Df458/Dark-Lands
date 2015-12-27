#include <curses.h>

#define MINIMUM_TERM_ROWS 24
#define MINIMUM_TERM_COLS 80

int main(int argc, char* argv[])
{
    /* Start Curses
     *
     * clear()  clears the terminal, giving us a blank canvas to work with
     * noecho() prevents getch() calls from displaying the typed character
     * cbreak() ensures that typed input is received immediately by curses
    */
    initscr();
    clear();
    noecho();
    cbreak();

    // Test the resolution of of our terminal
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    if(rows < MINIMUM_TERM_ROWS || cols < MINIMUM_TERM_COLS) {
        endwin();
        fprintf(stderr, "Error: Your terminal is too small.\nThis application expects dimentions of at least %d rows by %d columns.\n", MINIMUM_TERM_ROWS, MINIMUM_TERM_COLS);
        return 1;
    }

    // Print the title, then wait for a button press before exiting
    mvaddstr(3, cols / 2, "Cool Game Title");
    mvaddstr(rows / 2, cols / 2, "Press any key to continue");
    getch();

    // End Curses
    endwin();
    return 0;
}
