#include "actor.h"
#include "compat.h"
#include <stdlib.h>

actor* actor_new(uint8_t character, uint16_t y, uint16_t x) {
    actor* a = malloc(sizeof(actor));
    a->character = character;
    a->y = y;
    a->x = x;

    return a;
}
void actor_draw(actor* a) {
    mvaddch(a->y, a->x, a->character);
}
void actor_free(actor* a) {
    free(a);
}
