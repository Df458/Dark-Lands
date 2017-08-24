// Include the correct header for the library that we're using
#include "compat.h"
#include "actor.h"
#include "inventory.h"
#include "item.h"
#include "log.h"
#include "map.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

actor* player;
map* game_map;
WINDOW* map_window;

void draw_game() {
    wclear(map_window);
    map_draw(map_window, game_map, player);
    actor_draw(map_window, player, game_map, player);
    box(map_window, 0, 0);
    wmove(map_window, player->position.line, player->position.column); // Place the cursor on the player

    mvwprintw(map_window, 19, 3, "HP: %d/%d", player->hp, player->hp_max);

    log_draw();

    update_panels();
    doupdate();
}

int main() {
    putenv("PDC_COLS=80");
    putenv("PDC_LINES=30");
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

    log_init();

    game_map = map_new(30, 90);
    player = actor_new('@', (point){.line=(rand() % (game_map->lines - 1)) + 1, .column=(rand() % (game_map->columns - 1)) + 1});

    map_window = newwin(20, 80, 0, 0);
    PANEL* map_panel = new_panel(map_window);
    top_panel(map_panel);

    /* map_generate_drunkards(game_map, actor, 0.5); */
    map_generate_erode(game_map, player, 40);
    map_generate_river(game_map, (rand() % game_map->columns - 2) + 2);

    for(int i = 0; i < rand() % 10 + 5; ++i) {
        map* temp_map = map_new(10, 20);
        map_generate_erode(temp_map, NULL, 42);
        map_place_water(game_map, temp_map, (point){.line=(rand() % (game_map->lines - 10)) + 10, .column=(rand() % (game_map->columns - 10)) + 10});
        map_free(temp_map);
    }

    player->position = map_get_random_empty_tile(game_map);

    for(int i = 0; i < 20; ++i)
        give_item(&get_tile(game_map, map_get_random_empty_tile(game_map))->inv, create_potion(1));

    for(int i = 0; i < 50; ++i)
        give_item(&get_tile(game_map, map_get_random_empty_tile(game_map))->inv, create_trap(1));

    bool should_continue = true;
    while(should_continue) {
        draw_game();

        int input = getch();
        switch(input) {
            case 'q':
                should_continue = false;
                break;

            case ',':
                {
                    inventory* inv = &get_tile(game_map, player->position)->inv;
                    int32_t index = inventory_panel(TRUE, inv);
                    if(index >= 0) {
                        item* it = take_item(inv, index);
                        log_write("You pick up a %s", it->name);
                        give_item(&(player->inv), it);
                    }
                }
                break;

            case 'i':
                {
                    int32_t index = inventory_panel(FALSE, &(player)->inv);
                    if(index >= 0)
                        item_run_callback(&(player->inv), index, player, player->inv.item_list[index]->on_use);
                }
                break;

            case 's':
                stats_panel(player);
                break;

            case KEY_UP:
                actor_try_move(player, game_map, (point){.line=-1, .column=0});
                break;
            case KEY_DOWN:
                actor_try_move(player, game_map, (point){.line=1, .column=0});
                break;
            case KEY_LEFT:
                actor_try_move(player, game_map, (point){.line=0, .column=-1});
                break;
            case KEY_RIGHT:
                actor_try_move(player, game_map, (point){.line=0, .column=1});
                break;
        }
    }

    actor_free(player);
    map_free(game_map);

    log_shutdown();

    del_panel(map_panel);
    delwin(map_window);

    // Clean up
    endwin();

    return 0;
}
