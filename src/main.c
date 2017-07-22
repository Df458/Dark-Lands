// Include the correct header for the library that we're using
#include "compat.h"
#include "actor.h"
#include "map.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Initialize curses
    initscr();
    keypad(stdscr, true);
    srand(time(NULL));

    start_color();
    use_default_colors();

    init_color(COLOR_YELLOW, 400, 250, 200);
    init_color(COLOR_GREEN, 200, 300, 100);
    init_color(COLOR_BLUE, 100, 100, 500);
    init_color(9, 200, 400, 1000);
    init_color(10, 300, 350, 250);

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, 9, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, 10, COLOR_BLACK);

    map* test_map = map_new(30, 90);
    actor* actor = actor_new('@', (point){.line=(rand() % (test_map->lines - 1)) + 1, .column=(rand() % (test_map->columns - 1)) + 1});

    /* map_generate_drunkards(test_map, actor, 0.5); */
    map_generate_erode(test_map, actor, 40);
    map_generate_river(test_map, (rand() % test_map->columns - 2) + 2);

    for(int i = 0; i < rand() % 10 + 5; ++i) {
        map* temp_map = map_new(10, 20);
        map_generate_erode(temp_map, NULL, 42);
        map_place_water(test_map, temp_map, (point){.line=(rand() % (test_map->lines - 10)) + 10, .column=(rand() % (test_map->columns - 10)) + 10});
        map_free(temp_map);
    }

    bool should_continue = true;
    while(should_continue) {
        clear();
        map_draw(test_map, actor);
        actor_draw(actor, test_map, actor);
        move(10, 0);              // Place the cursor at a specified location
        /* move(actor->y, actor->x); // Place the cursor on the player */

        int input = getch();
        switch(input) {
            case 'q':
                should_continue = false;
                break;
            
            case KEY_UP:
                if(actor->position.line > 0 && !get_tile(test_map, point_add(actor->position, (point){.line=-1, .column=0}))->solid)
                    actor->position = point_add(actor->position, (point){.line=-1, .column=0});
                break;
            case KEY_DOWN:
                if(actor->position.line < test_map->lines - 1 && !get_tile(test_map, point_add(actor->position, (point){.line=1, .column=0}))->solid)
                    actor->position = point_add(actor->position, (point){.line=1, .column=0});
                break;
            case KEY_LEFT:
                if(actor->position.column > 0 && !get_tile(test_map, point_add(actor->position, (point){.line=0, .column=-1}))->solid)
                    actor->position = point_add(actor->position, (point){.line=0, .column=-1});
                break;
            case KEY_RIGHT:
                if(actor->position.column < test_map->columns - 1 && !get_tile(test_map, point_add(actor->position, (point){.line=0, .column=1}))->solid)
                    actor->position = point_add(actor->position, (point){.line=0, .column=1});
                break;
        }
    }

    actor_free(actor);
    map_free(test_map);

    // Clean up
    endwin();

    return 0;
}
