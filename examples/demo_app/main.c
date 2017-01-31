#include "compat.h"
#include "animation.h"

#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

const char* menu_items[] = {
    "Controls",
    "Animation",
    "Colors",
    "Styles",
    "Charmap",
};
const uint8_t MENU_SIZE = 5;

const char spinner[] = {
    '|',
    '/',
    '-',
    '\\',
};

void init_colors() {
    if(!has_colors()) {
        endwin();
        fprintf(stderr, "This demo app requires color support!");
        exit(1);
    }

    start_color();
    for(int i = 0; i < COLORS; ++i) {
        init_pair(i, i, 0);
    }
}

WINDOW* add_window(int lines, int cols, int y, int x) {
    WINDOW* new_win = newwin(lines, cols, y, x);
    wborder(new_win, 0, 0, 0, 0, 0, 0, 0, 0);
    keypad(new_win, TRUE);
    nodelay(new_win, TRUE);

    return new_win;
}

void controls_draw(WINDOW* win) {
    mvwprintw(win, 2, 3, "Up/Down:    Navigate Menu");
    mvwprintw(win, 3, 3, "Left/Right: Change Tick Delay");
    mvwprintw(win, 4, 3, "Q:          Quit");
}

void color_draw(WINDOW* win) {
    mvwprintw(win, 1, 1, "Max Colors: %d", COLORS);
    mvwprintw(win, 2, 1, "Max Color Pairs: %d (%d per color)", COLOR_PAIRS, COLOR_PAIRS / COLORS);
    mvwprintw(win, 3, 1, "Colormap:");
    for(int i = 0; i < COLORS && i / 8 < 38; ++i) {
        wattron(win, COLOR_PAIR(i));
        mvwaddch(win, i / 8 + 4, i % 8 + 1, 219);
    }
}

void styles_draw(WINDOW* win) {
    wattron(win, A_NORMAL);
    mvwaddstr(win, 1, 1, "Normal");
    wattroff(win, A_NORMAL);
    wattron(win, A_STANDOUT);
    mvwaddstr(win, 2, 1, "Standout");
    wattroff(win, A_STANDOUT);
    wattron(win, A_UNDERLINE);
    mvwaddstr(win, 3, 1, "Underline");
    wattroff(win, A_UNDERLINE);
    wattron(win, A_REVERSE);
    mvwaddstr(win, 4, 1, "Reverse");
    wattroff(win, A_REVERSE);
    wattron(win, A_BLINK);
    mvwaddstr(win, 5, 1, "Blink");
    wattroff(win, A_BLINK);
    wattron(win, A_DIM);
    mvwaddstr(win, 6, 1, "Dim");
    wattroff(win, A_DIM);
    wattron(win, A_BOLD);
    mvwaddstr(win, 7, 1, "Bold");
    wattroff(win, A_BOLD);
    wattron(win, A_PROTECT);
    mvwaddstr(win, 8, 1, "Protected");
    wattroff(win, A_PROTECT);
    wattron(win, A_INVIS);
    mvwaddstr(win, 9, 1, "Invisible");
    wattroff(win, A_INVIS);
    wattron(win, A_ALTCHARSET);
    mvwaddstr(win, 10, 1, "Alt. Charset");
    wattroff(win, A_ALTCHARSET);
    wattron(win, A_ITALIC);
    mvwaddstr(win, 11, 1, "Italic");
    wattroff(win, A_ITALIC);
}

void charmap_draw(WINDOW* win) {
    for(int i = 0; i < 95; ++i) {
        mvwprintw(win, 1, 1, "ASCII");
        if(i % 16 == 0)
            mvwprintw(win, i / 16 + 2, 1, "%03d-%03d", i + 33, i + 49);
        mvwaddch(win, i / 16 + 2, i % 16 + 9, i + 33);
    }
    for(int i = 0; i < 128; ++i) {
        mvwprintw(win, 96 / 16 + 4, 1, "Extended ASCII");
        if(i % 16 == 0)
            mvwprintw(win, 96 / 16 + i / 16 + 5, 1, "%03d-%03d", i + 128, i + 144);
        mvwaddch(win, 96 / 16 + i / 16 + 5, i % 16 + 9, i + 128);
    }
}

int main(int argc, char* argv[])
{
    // If we're using SDL, tell curses the desired size of our window
#ifndef TERMBUILD
    putenv("PDC_LINES=40");
    putenv("PDC_COLS=120");
#endif // TERMBUILD

    // Init data
    uint16_t tick_delay = 16;     // Control the redraw rate
    bool should_exit = false;     // Used to exit the main loop
    uint16_t cursor_position = 0; // Tracks the active menu item
    uint8_t tick_index = 0;       // Used to animate the tick spinner

    // Init and configure curses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // Prepare colors and drawing windows
    init_colors();
    WINDOW* menu_win = add_window(40, 40, 0, 0);
    WINDOW* demo_win = add_window(40, 80, 0, 40);

    // Main loop
    while(!should_exit) {
        // Handle input
        int input = wgetch(menu_win);
        switch(input) {
            case 'q':
            case KEY_EXIT:
                should_exit = true;
                break;
            case KEY_UP:
                if(cursor_position > 0) {
                    cursor_position--;
                    wclear(demo_win);
                    wborder(demo_win, 0, 0, 0, 0, 0, 0, 0, 0);
                }
                break;
            case KEY_DOWN:
                if(cursor_position < MENU_SIZE - 1) {
                    cursor_position++;
                    wclear(demo_win);
                    wborder(demo_win, 0, 0, 0, 0, 0, 0, 0, 0);
                }
                break;
            case KEY_LEFT:
                if(tick_delay > 16)
                    tick_delay -= 8;
                break;
            case KEY_RIGHT:
                if(tick_delay < 128)
                    tick_delay += 8;
                break;
        }

        // Draw the menu
        for(uint16_t i = 0; i < MENU_SIZE; ++i) {
            if(i == cursor_position)
                wattron(menu_win, A_STANDOUT);
            mvwaddstr(menu_win, i + 2, 2, menu_items[i]);
            if(i == cursor_position)
                wattroff(menu_win, A_STANDOUT);
        }

        // Draw the spinner and tick delay
        if(tick_index < 4)
            ++tick_index;
        else
            tick_index = 0;
        mvwaddch(menu_win,  MENU_SIZE + 3, 2, spinner[tick_index % 4]);
        mvwprintw(menu_win, MENU_SIZE + 3, 4, "Tick delay: %3d", tick_delay);

        // Draw the demo
        switch(cursor_position) {
            case 0:
                controls_draw(demo_win);
                break;
            case 1:
                animation_draw(demo_win);
                break;
            case 2:
                color_draw(demo_win);
                break;
            case 3:
                styles_draw(demo_win);
                break;
            case 4:
                charmap_draw(demo_win);
                break;
        }

        // Update the windows and wait
        wrefresh(menu_win);
        wrefresh(demo_win);
        msleep(tick_delay);
    }

    endwin();

    return 0;
}
