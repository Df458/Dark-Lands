// Include the correct header for the library that we're using
#include "compat.h"
#include "actor.h"
#include "map.h"
#include <stdbool.h>

int main() {
    // Initialize curses
    initscr();
    keypad(stdscr, true);

    map* test_map = map_new(10, 10);
    actor* actor = actor_new('@', 3, 3);

    bool should_continue = true;
    while(should_continue) {
        map_draw(test_map);
        actor_draw(actor);

        int input = getch();
        switch(input) {
            case 'q':
                should_continue = false;
                break;
            
            case KEY_UP:
                if(actor->y > 0)
                    actor->y--;
                break;
            case KEY_DOWN:
                if(actor->y < test_map->lines - 1)
                    actor->y++;
                break;
            case KEY_LEFT:
                if(actor->x > 0)
                    actor->x--;
                break;
            case KEY_RIGHT:
                if(actor->x < test_map->columns - 1)
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
