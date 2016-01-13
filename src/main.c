#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include "draw.h"

#define MINIMUM_TERM_ROWS 24
#define MINIMUM_TERM_COLS 80
#define MINIMUM_COLORS    16
#define MINIMUM_PAIRS     16

int main(int argc, char* argv[])
{
    // Seed the RNG
    srand(time(NULL));

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

    // Test the color capabilities of our terminal
    if(!has_colors()) {
        endwin();
        fprintf(stderr, "Error: Your terminal does not support color output.\nThis application must have color output to run.\n");
        return 1;
    }

    start_color();

    // If they support color but lack full support, warn them and continue
    if(!can_change_color()) {
        endwin();
        fprintf(stderr, "Warning: Your terminal cannot change color definitions.\nThis may impact the game's aesthetic.\nPress any key to continue.\n");
        getch();
        refresh();
    }
    if(COLORS < MINIMUM_COLORS || COLOR_PAIRS < MINIMUM_PAIRS) {
        endwin();
        fprintf(stderr, "Warning: Your terminal lacks sufficient color support to use this game's full range of color.\nThis may impact the game's aesthetic.\nPress any key to continue.\n");
        getch();
        refresh();
    }

    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    color_set(1, 0);
    draw_stars(stdscr);
    color_set(2, 0);
    draw_rect(stdscr, rows / 2 + 3, 0, rows / 2 - 3, cols, '#');

    // Reset our color
    color_set(0, 0);

    // Print the title, then wait for a button press before exiting
    const char* title_str = "Cool Game Title";
    draw_text_aligned(stdscr, 3, "Cool Game Title", ALIGN_CENTER);
    draw_text_aligned(stdscr, rows / 2, "Press any key to continue", ALIGN_CENTER);
    getch();

    // End Curses
    endwin();
    return 0;
}
