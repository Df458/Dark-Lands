#ifndef MAP_H
#define MAP_H
#include <inttypes.h>

typedef struct tile {
    uint8_t character;
    int color_pair;
} tile;

typedef struct map {
    tile** data;
    uint16_t lines;
    uint16_t columns;
} map;

map* map_new(uint16_t lines, uint16_t columns);
void map_draw(map* m);
tile* get_tile(map* m, uint16_t l, uint16_t c);
void map_free(map* m);

#endif
