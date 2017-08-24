#ifndef MAP_H
#define MAP_H
#include "compat.h"
#include "inventory.h"
#include "point.h"

typedef struct tile {
    uint8_t character;
    int color_pair;
    bool solid;
    inventory inv;
} tile;

typedef struct map {
    tile** data;
    uint16_t lines;
    uint16_t columns;
} map;

typedef struct actor actor;

map* map_new(uint16_t lines, uint16_t columns);
void map_generate_drunkards(map* m, actor* a, float map_percent);
void map_generate_erode(map* m, actor* a, int ratio);
void map_generate_river(map* m, uint16_t column);
void map_place_water(map* m, map* src, point loc);
void map_draw(WINDOW* win, map* m, actor* focus);
tile* get_tile(map* m, point p);
point map_get_random_empty_tile(map* m);
void map_free(map* m);

#endif
