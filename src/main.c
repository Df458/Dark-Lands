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

    map* test_map = map_new(100, 100);
    actor* actor = actor_new('@', (rand() % (test_map->lines - 1)) + 1, (rand() % (test_map->columns - 1)) + 1);

    /* map_generate_drunkards(test_map, actor); */
    map_generate_erode(test_map, actor);

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
                if(actor->y > 0 && !get_tile(test_map, actor->y - 1, actor->x)->solid)
                    actor->y--;
                break;
            case KEY_DOWN:
                if(actor->y < test_map->lines - 1 && !get_tile(test_map, actor->y + 1, actor->x)->solid)
                    actor->y++;
                break;
            case KEY_LEFT:
                if(actor->x > 0 && !get_tile(test_map, actor->y, actor->x - 1)->solid)
                    actor->x--;
                break;
            case KEY_RIGHT:
                if(actor->x < test_map->columns - 1 && !get_tile(test_map, actor->y, actor->x + 1)->solid)
                    actor->x++;
                break;
        }
    }

    actor_free(actor);
    map_free(test_map);

    // Clean up
    endwin();

    return 0;
}
