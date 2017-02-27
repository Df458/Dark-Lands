#include "actor.h"
#include "map.h"
#include "compat.h"
#include <stdlib.h>

actor* actor_new(uint8_t character, uint16_t y, uint16_t x) {
    actor* a = malloc(sizeof(actor));
    a->character = character;
    a->y = y;
    a->x = x;

    return a;
}
void actor_draw(actor* a, map* m, actor* focus) {
    int offset_y = focus->y - (VIEW_LINES / 2);
    int offset_x = focus->x - (VIEW_COLS / 2);
    if(m->lines <= VIEW_LINES) {
        offset_y = -(VIEW_LINES - m->lines) / 2;
    }
    if(m->columns <= VIEW_COLS) {
        offset_x = -(VIEW_COLS - m->columns) / 2;
    }

    if(a->y - offset_y < 0 || a->y - offset_y > VIEW_LINES || a->x - offset_x < 0 || a->x - offset_x > VIEW_COLS)
        return;

    mvaddch(a->y - offset_y, a->x - offset_x, a->character);
}
void actor_free(actor* a) {
    free(a);
}
