#include "map.h"
#include "actor.h"
#include <stdlib.h>

map* map_new(uint16_t lines, uint16_t columns) {
    map* m = malloc(sizeof(map));
    m->lines = lines;
    m->columns = columns;
    m->data = calloc(lines, sizeof(tile*));
    for(int i = 0; i < lines; ++i) {
        m->data[i] = calloc(columns, sizeof(tile));
        for(int j = 0; j < columns; ++j) {
            m->data[i][j].character = 219;
            m->data[i][j].solid = TRUE;
        }
    }

    return m;
}
void map_generate_drunkards(map* m, actor* a) {
    uint16_t y = a->y; // We want to start at the player's location,
    uint16_t x = a->x; // in order to guarantee that they spawn in the map.
    uint16_t count = 0;

    // You can change the number that we multiply to adjust how much open space
    // the map has.
    while(count < m->lines * m->columns * 0.2) {
        if(m->data[y][x].solid) {
            m->data[y][x].solid = FALSE;
            m->data[y][x].character = '.';

            count++;
        }

        // Move in a random direction, avoiding the edges
        if(!(rand() % 2)) {
            if(y > 1 && (!(rand() % 2) || y + 1 >= m->lines - 2))
                y--;
            else
                y++;
        } else {
            if(x > 1 && (!(rand() % 2) || x + 1 >= m->columns - 2))
                x--;
            else
                x++;
        }
    }
}
void map_generate_erode(map* m, actor* a) {
    int passcount         = 3;  // How many passes to perform
    int ratio             = 45; // Percentage of solid tiles to shoot for each pass
    int min_neighbors     = 5;  // How many neighbors are needed to stay non-solid
    int min_neighbors_non = 3;  // How many neighbors are needed to stay solid

    // Empty the map, but keep the edges solid
    for(int i = 0; i < m->lines; ++i) {
        for(int j = 0; j < m->columns; ++j) {
            if(i == 0 || i == m->lines - 1 || j == 0 || j == m->columns - 1) {
                m->data[i][j].solid = TRUE;
                m->data[i][j].character = 219;
            } else {
                m->data[i][j].solid = FALSE;
                m->data[i][j].character = '.';
            }
        }
    }

    // Passes
    for(int pass = 0; pass < passcount; ++pass) {
        // Fill empty spaces with some percentage (ratio) of solid tiles
        for(int i = 1; i < m->lines - 1; ++i) {
            for(int j = 1; j < m->columns - 1; ++j) {
                if(!m->data[i][j].solid) {
                    m->data[i][j].solid = rand() % 100 < ratio;
                    m->data[i][j].character = m->data[i][j].solid ? 219 : '.';
                }
            }
        }

        // Run the Cellular Automaton
        for(int i = 1; i < m->lines - 1; ++i) {
            for(int j = 1; j < m->columns - 1; ++j) {
                if(m->data[i][j].solid) {
                    // Get the number of similar neighbors
                    int neighbors = 0;
                    for(int k = -1; k < 2; ++k) {
                        for(int l = -1; l < 2; ++l) {
                            if(m->data[i + k][j + l].solid)
                                neighbors++;
                        }
                    }

                    m->data[i][j].solid = neighbors >= min_neighbors;
                    m->data[i][j].character = m->data[i][j].solid ? 219 : '.';
                } else {
                    // Get the number of similar neighbors
                    int neighbors = 0;
                    for(int k = -1; k < 2; ++k) {
                        for(int l = -1; l < 2; ++l) {
                            if(!m->data[i + k][j + l].solid)
                                neighbors++;
                        }
                    }

                    m->data[i][j].solid = neighbors < min_neighbors_non;
                    m->data[i][j].character = m->data[i][j].solid ? 219 : '.';
                }
            }
        }
    }

    // Second cleanup pass
    for(int i = 1; i < m->lines - 1; ++i) {
        for(int j = 1; j < m->columns - 1; ++j) {
            if(m->data[i][j].solid) {
                int neighbors = 0;
                for(int k = -1; k < 2; ++k) {
                    for(int l = -1; l < 2; ++l) {
                        if(m->data[i + k][j + l].solid)
                            neighbors++;
                    }
                }

                m->data[i][j].solid = neighbors >= min_neighbors;
                m->data[i][j].character = m->data[i][j].solid ? 219 : '.';
            } else {
                int neighbors = 0;
                for(int k = -1; k < 2; ++k) {
                    for(int l = -1; l < 2; ++l) {
                        if(!m->data[i + k][j + l].solid)
                            neighbors++;
                    }
                }

                m->data[i][j].solid = neighbors < min_neighbors_non;
                m->data[i][j].character = m->data[i][j].solid ? 219 : '.';
            }
        }
    }
}
void map_draw(map* m, actor* focus) {
    // Get the offset needed to put the focus in the center of the view
    int offset_y = focus->y - (VIEW_LINES / 2);
    int offset_x = focus->x - (VIEW_COLS / 2);

    // Adjust the offset so that small maps are centered
    if(m->lines <= VIEW_LINES) {
        offset_y = -(VIEW_LINES - m->lines) / 2;
    }
    if(m->columns <= VIEW_COLS) {
        offset_x = -(VIEW_COLS - m->columns) / 2;
    }
    
    // Draw with the calculated offset
    for(int i = (offset_y > 0 ? offset_y : 0); i < (offset_y + VIEW_LINES <= m->lines ? offset_y + VIEW_LINES : m->lines); ++i) {
        for(int j = (offset_x > 0 ? offset_x : 0); j < (offset_x + VIEW_COLS <= m->columns ? offset_x + VIEW_COLS : m->columns); ++j) {
            mvaddch(i - offset_y, j - offset_x, m->data[i][j].character);
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
