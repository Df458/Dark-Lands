#include "actor.h"
#include "map.h"
#include "compat.h"
#include <stdlib.h>

actor* actor_new(uint8_t character, point p) {
    actor* a = malloc(sizeof(actor));
    a->character = character;
    a->position = p;

    return a;
}
void actor_draw(actor* a, map* m, actor* focus) {
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

    color_set(a->color_pair, NULL);
    mvaddch(a->position.line - offset_y, a->position.column - offset_x, a->character);
}
void actor_free(actor* a) {
    free(a);
}
