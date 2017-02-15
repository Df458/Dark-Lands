#ifndef CHARACTER_H
#define CHARACTER_H
#include <inttypes.h>

typedef struct actor {
    uint8_t character;
    uint16_t y;
    uint16_t x;
    int color_pair;
} actor;

actor* actor_new(uint8_t character, uint16_t y, uint16_t x);
void actor_draw(actor* a);
void actor_free(actor* a);

#endif
