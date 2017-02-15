#include "map.h"
#include "compat.h"
#include <stdlib.h>

map* map_new(uint16_t lines, uint16_t columns) {
    map* m = malloc(sizeof(map));
    m->lines = lines;
    m->columns = columns;
    m->data = calloc(lines, sizeof(tile*));
    for(int i = 0; i < lines; ++i) {
        m->data[i] = calloc(columns, sizeof(tile));
        for(int j = 0; j < columns; ++j) {
            m->data[i][j].character = '.';
        }
    }

    return m;
}
void map_draw(map* m) {
    for(int i = 0; i < m->lines; ++i) {
        for(int j = 0; j < m->columns; ++j) {
            mvaddch(i, j, m->data[i][j].character);
        }
    }
}
tile* get_tile(map* m, uint16_t l, uint16_t c) {
    return &m->data[l][c];
}
void map_free(map* m) {
    for(int i = 0; i < m->lines; ++i) {
        free(m->data[i]);
    }
    free(m);
}
