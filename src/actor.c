#include "actor.h"
#include "log.h"
#include "map.h"
#include <stdlib.h>

actor* actor_new(uint8_t character, point p) {
    actor* a = malloc(sizeof(actor));
    a->character = character;
    a->position = p;
    a->level = 1;
    a->xp = 0;
    a->hp_max = 100;
    a->hp = a->hp_max;
    a->attack = 10;
    a->defense = 10;
    a->inv.item_count = 0;
    a->inv.item_slots = 1;
    a->inv.item_list  = malloc(sizeof(item*));

    return a;
}
void actor_draw(WINDOW* win, actor* a, map* m, actor* focus) {
    int offset_y = focus->position.line - (VIEW_LINES / 2);
    int offset_x = focus->position.column - (VIEW_COLS / 2);
    if(m->lines <= VIEW_LINES) {
        offset_y = -(VIEW_LINES - m->lines) / 2;
    }
    if(m->columns <= VIEW_COLS) {
        offset_x = -(VIEW_COLS - m->columns) / 2;
    }

    if(a->position.line - offset_y < 0 || a->position.line - offset_y > VIEW_LINES || a->position.column - offset_x < 0 || a->position.column - offset_x > VIEW_COLS)
        return;

    wcolor_set(win, a->color_pair, NULL);
    mvwaddch(win, a->position.line - offset_y, a->position.column - offset_x, a->character);
}
void actor_free(actor* a) {
    inventory_cleanup(&(a->inv));
    free(a);
}

uint8_t actor_try_move(actor* a, map* m, point offset) {
    tile* t = get_tile(m, point_add(a->position, offset));
    if(t != NULL && !t->solid) {
        a->position = point_add(a->position, offset);
        for(uint16_t i = 0; i < t->inv.item_count; ++i) {
            item_run_callback(&(t->inv), i, a, t->inv.item_list[i]->on_step);
        }

        if(t->inv.item_count > 0) {
            if(t->inv.item_count == 1)
                log_write("You see a %s on the ground", t->inv.item_list[0]->name);
            else
                log_write("You see a several items on the ground");
        }

        return TRUE;
    }

    return FALSE;
}

void stats_panel(actor* a) {
    WINDOW* stats_window = newwin(25, 40, 0, 40);
    PANEL* stats_panel = new_panel(stats_window);

    top_panel(stats_panel);

    do {
        wclear(stats_window);
        box(stats_window, 0, 0);

        mvwaddch(stats_window, 22, 0, 195);
        for(int i = 1; i < 39; ++i)
            mvwaddch(stats_window, 22, i, 196);
        mvwaddch(stats_window, 22, 39, 180);
        mvwaddstr(stats_window, 23, 1, "q to exit");

        mvwprintw(stats_window, 1, 1, "Level:\t%u (%u/%u)", a->level, a->xp, 100);
        mvwprintw(stats_window, 3, 1, "Health:\t%u/%u", a->hp, a->hp_max);
        mvwprintw(stats_window, 4, 1, "Attack:\t%u", a->attack);
        mvwprintw(stats_window, 5, 1, "Defense:\t%u", a->defense);

        update_panels();
        doupdate();
    } while(getch() != 'q');

    del_panel(stats_panel);
    delwin(stats_window);
}
