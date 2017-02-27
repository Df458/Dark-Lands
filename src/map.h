#ifndef MAP_H
#define MAP_H
#include "compat.h"
#include <inttypes.h>

typedef struct tile {
    uint8_t character;
    int color_pair;
    bool solid;
} tile;

typedef struct map {
    tile** data;
    uint16_t lines;
    uint16_t columns;
} map;

typedef struct actor actor;

map* map_new(uint16_t lines, uint16_t columns);
void map_generate_drunkards(map* m, actor* a);
void map_generate_erode(map* m, actor* a);
void map_draw(map* m, actor* focus);
tile* get_tile(map* m, uint16_t l, uint16_t c);
void map_free(map* m);

#endif
