#ifndef CHARACTER_H
#define CHARACTER_H
#include <inttypes.h>
#include "compat.h"
#include "inventory.h"
#include "point.h"

typedef struct actor {
    uint8_t character;
    point position;
    int color_pair;

    uint16_t level;
    uint16_t xp;
    uint16_t hp;
    uint16_t hp_max;
    uint16_t attack;
    uint16_t defense;

    inventory inv;
} actor;

typedef struct map map;

actor* actor_new(uint8_t character, point p);
void actor_draw(WINDOW* win, actor* a, map* m, actor* focus);
void actor_free(actor* a);

void stats_panel(actor* a);

uint8_t actor_try_move(actor* a, map* m, point offset);

#endif
