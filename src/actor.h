#ifndef CHARACTER_H
#define CHARACTER_H
#include <inttypes.h>
#include "point.h"

typedef struct actor {
    uint8_t character;
    point position;
    int color_pair;
} actor;

typedef struct map map;

actor* actor_new(uint8_t character, point p);
void actor_draw(actor* a, map* m, actor* focus);
void actor_free(actor* a);

#endif
