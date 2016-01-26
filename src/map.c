#include <curses.h>
#include <stdlib.h>
#include "map.h"

typedef struct tile
{
    int tile_char;
    int tile_pair;
} tile;

int map_rows, map_cols;
tile* map_tiles = 0;

void generate_map()
{
    // Set map dimensions to match the screen
    getmaxyx(stdscr, map_rows, map_cols);

    // Create a new map, deleting an old one if it exists already
    if(map_tiles)
        free(map_tiles);
    map_tiles = calloc(map_rows * map_cols, sizeof(tile));
    for(int i = 0; i < map_rows; ++i) {
        for(int j = 0; j < map_cols; ++j) {
            map_tiles[i * map_rows + j].tile_pair = rand() % 3;
            map_tiles[i * map_rows + j].tile_char = (rand() % 93) + 32;
        }
    }
}

void draw_map()
{
    for(int i = 0; i < map_rows; ++i) {
        move(i, 0);
        for(int j = 0; j < map_cols; ++j) {
            attron(COLOR_PAIR(map_tiles[i * map_rows + j].tile_pair));
            addch(map_tiles[i * map_rows + j].tile_char);
        }
    }
}
