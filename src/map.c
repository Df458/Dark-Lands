#include "map.h"
#include "actor.h"
#include <stdlib.h>

const char grass_chars[] = {
    '"',
    '\'',
    ',',
    '.',
    '"'
};

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
            m->data[i][j].inv.item_list = malloc(sizeof(item));
            m->data[i][j].inv.item_count = 0;
            m->data[i][j].inv.item_slots = 1;
        }
    }

    return m;
}
void map_generate_drunkards(map* m, actor* a, float map_percent) {
    // We want to start at the player's location,
    // in order to guarantee that they spawn in the map.
    point pos = a->position;
    uint16_t count = 0;
    uint16_t goal = m->lines * m->columns * map_percent;

    // You can change the number that we multiply to adjust how much open space
    // the map has.
    for(uint16_t i = 0; i < goal * 2 && count < goal; ++i) {
        if(m->data[pos.line][pos.column].solid) {
            m->data[pos.line][pos.column].solid = FALSE;
            m->data[pos.line][pos.column].character = '.';

            count++;
        }

        // Move in a random direction, avoiding the edges
        if(!(rand() % 2)) {
            if(pos.line > 1 && (!(rand() % 2) || pos.line + 1 >= m->lines - 2))
                pos.line--;
            else
                pos.line++;
        } else {
            if(pos.column > 1 && (!(rand() % 2) || pos.column + 1 >= m->columns - 2))
                pos.column--;
            else
                pos.column++;
        }
    }
}

int group_flood(int** groups, point p, point corner, int group_index) {
    point* remaining = calloc(10, sizeof(point));
    uint16_t remaining_data_size = 10;
    uint16_t remaining_count = 1;
    int counter = 0;
    remaining[0] = p;

    while(remaining_count > 0) {
        remaining_count--;
        counter++;
        // Note: Because this value is copied here, changing it later won't effect cur.
        point cur = remaining[remaining_count];

        // If we don't have enough slots for all adjacent tiles, resize
        if(remaining_count + 4 > remaining_data_size) {
            remaining_data_size += 4;
            remaining = realloc(remaining, remaining_data_size * sizeof(point));
        }

        groups[cur.line][cur.column] = group_index;
        if(cur.line > 0 && groups[cur.line - 1][cur.column] == -1) {
            remaining[remaining_count] = (point){ .line = cur.line - 1, .column = cur.column };
            remaining_count++;
        }
        if(cur.line < corner.line - 1 && groups[cur.line + 1][cur.column] == -1) {
            remaining[remaining_count] = (point){ .line = cur.line + 1, .column = cur.column };
            remaining_count++;
        }
        if(cur.column > 0 && groups[cur.line][cur.column - 1] == -1) {
            remaining[remaining_count] = (point){ .line = cur.line, .column = cur.column - 1 };
            remaining_count++;
        }
        if(cur.column < corner.column - 1 && groups[cur.line][cur.column + 1] == -1) {
            remaining[remaining_count] = (point){ .line = cur.line, .column = cur.column + 1 };
            remaining_count++;
        }
    }

    free(remaining);

    return counter;
}
void map_generate_erode(map* m, actor* a, int ratio) {
    int passcount         = 3;  // How many passes to perform
    int min_neighbors     = 5;  // How many neighbors are needed to stay non-solid
    int min_neighbors_non = 3;  // How many neighbors are needed to stay solid

    // Empty the map, but keep the edges solid
    for(int i = 0; i < m->lines; ++i) {
        for(int j = 0; j < m->columns; ++j) {
            if(i == 0 || i == m->lines - 1 || j == 0 || j == m->columns - 1) {
                m->data[i][j].solid = TRUE;
                m->data[i][j].color_pair = 5;
                m->data[i][j].character = 219;
            } else {
                m->data[i][j].solid = FALSE;
                m->data[i][j].color_pair = 4;
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
                    m->data[i][j].color_pair = m->data[i][j].solid ? 5 : 4;
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
                    m->data[i][j].color_pair = m->data[i][j].solid ? 5 : 4;
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
                    m->data[i][j].color_pair = m->data[i][j].solid ? 5 : 4;
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
                m->data[i][j].color_pair = m->data[i][j].solid ? 5 : 4;
                m->data[i][j].character = m->data[i][j].solid ? 219 : '.';
                if(rand() % 15 == 0 && !m->data[i][j].solid) {
                    m->data[i][j].color_pair = 3;
                    m->data[i][j].character = grass_chars[rand() % 5];
                }
            } else {
                int neighbors = 0;
                for(int k = -1; k < 2; ++k) {
                    for(int l = -1; l < 2; ++l) {
                        if(!m->data[i + k][j + l].solid)
                            neighbors++;
                    }
                }

                m->data[i][j].solid = neighbors < min_neighbors_non;
                m->data[i][j].color_pair = m->data[i][j].solid ? 5 : 4;
                m->data[i][j].character = m->data[i][j].solid ? 219 : '.';
                if(rand() % 15 == 0 && !m->data[i][j].solid) {
                    m->data[i][j].color_pair = 3;
                    m->data[i][j].character = grass_chars[rand() % 5];
                }
            }
        }
    }

    int** group = calloc(m->lines, sizeof(int*));
    for(int i = 0; i < m->lines; ++i) {
        group[i] = calloc(m->columns, sizeof(int));
        for(int j = 0; j < m->columns; ++j) {
            if(m->data[i][j].solid)
                group[i][j] = 0;
            else
                group[i][j] = -1;
        }
    }

    int group_index = 1;
    int largest_group = -1;
    int largest_group_size = 0;
    for(int i = 0; i < m->lines; ++i) {
        for(int j = 0; j < m->columns; ++j) {
            if(group[i][j] < 0) {
                int size = group_flood(group, (point){.line=i,.column=j}, (point){ .line=m->lines, .column=m->columns }, group_index);
                if(size > largest_group_size) {
                    largest_group = group_index;
                    largest_group_size = size;
                }
                group_index++;
            }
        }
    }

    for(int i = 0; i < m->lines; ++i) {
        for(int j = 0; j < m->columns; ++j) {
            if(group[i][j] != largest_group) {
                m->data[i][j].solid = TRUE;
                m->data[i][j].color_pair = 5;
                m->data[i][j].character = 219;
            }
        }
    }

    for(int i = 0; i < m->lines; ++i)
        free(group[i]);
    free(group);

    if(a) {
        while(m->data[a->position.line][a->position.column].solid) {
            a->position = (point){.line=(rand() % (m->lines - 1)) + 1, .column=(rand() % (m->columns - 1)) + 1};
        }
    }
}
void map_place_water(map* m, map* src, point loc) {
    point ul = (point){ loc.line - (src->lines / 2), loc.column - (src->columns / 2) };
    point br = (point){ loc.line + (src->lines / 2), loc.column + (src->columns / 2) };
    point offset = (point) {0};

    if(loc.line < src->lines / 2) {
        offset.line = (src->lines / 2) - loc.line;
        ul.line = 0;
    }
    if(loc.column < src->columns / 2) {
        offset.column = (src->columns / 2) - loc.column;
        ul.column = 0;
    }
    if(br.line >= m->lines)
        br.line = m->lines - 1;
    if(br.column >= m->columns)
        br.column = m->columns - 1;

    for(uint16_t i = 0; i < br.line - ul.line; ++i) {
        for(uint16_t j = 0; j < br.column - ul.column; ++j) {
            if(!src->data[i + offset.line][j + offset.column].solid) {
                m->data[ul.line + i][ul.column + j].character = '~';
                m->data[ul.line + i][ul.column + j].color_pair = rand() % 3 + 1;
                m->data[ul.line + i][ul.column + j].solid = TRUE;
            }
        }
    }
}
void map_generate_river(map* m, uint16_t column) {
    bool split = FALSE;
    point split_point;
    int split_mod = 1;
    for(int i = 0; i < m->lines; ++i) {
        int width = rand() % 4 + 2;
        int min = -width / 2;
        if(column + min <= 0)
            min = 1 - column;
        int max = width / 2;
        if(column + max >= m->columns - 1)
            max = m->columns - 2 - column;
        for(int j = min; j < max; ++j) {
            m->data[i][column + j].character = '~';
            m->data[i][column + j].color_pair = rand() % 3 + 1;
            m->data[i][column + j].solid = TRUE;
        }
        column += rand() % 3 - 1;

        if(rand() % 10 == 0 && !split) {
            split = TRUE;
            if(rand() % 2 == 0)
                split_point = (point){ .line = i, .column = column + max + 1 };
            else {
                split_point = (point){ .line = i, .column = column + min - 1 };
                split_mod = -1;
            }
        }
    }

    if(split) {
        column = split_point.column;
        for(int i = split_point.line; i < m->lines; ++i) {
            int width = rand() % 4 + 2;
            int min = -width / 2;
            if(column + min <= 0)
                min = 1 - column;
            int max = width / 2;
            if(column + max >= m->columns - 1)
                max = m->columns - 2 - column;
            for(int j = min; j < max; ++j) {
                m->data[i][column + j].character = '~';
                m->data[i][column + j].color_pair = rand() % 3 + 1;
                m->data[i][column + j].solid = TRUE;
            }
            column += rand() % 3 * split_mod;
        }
    }
}
void map_draw(WINDOW* win, map* m, actor* focus) {
    // Get the offset needed to put the focus in the center of the view
    int offset_y = focus->position.line - (VIEW_LINES / 2);
    int offset_x = focus->position.column - (VIEW_COLS / 2);

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
            if(m->data[i][j].inv.item_count == 0) {
                wcolor_set(win, m->data[i][j].color_pair, NULL);
                mvwaddch(win, i - offset_y, j - offset_x, m->data[i][j].character);
            } else {
                item* it = m->data[i][j].inv.item_list[m->data[i][j].inv.item_count - 1];
                wcolor_set(win, it->color_pair, NULL);
                mvwaddch(win, i - offset_y, j - offset_x, it->character);
            }
        }
    }
}
tile* get_tile(map* m, point p) {
    if(p.line < 0 || p.column < 0 || p.line >= m->lines || p.column >= m->columns)
        return NULL;
    return &m->data[p.line][p.column];
}
point map_get_random_empty_tile(map* m) {
    point p;

    do {
        p = (point){ .line=(rand() % m->lines - 1) + 1, .column=(rand() % m->columns - 1) + 1 };
    } while(m->data[p.line][p.column].solid);

    return p;
}
void map_free(map* m) {
    for(int i = 0; i < m->lines; ++i) {
        free(m->data[i]);
        for(int j = 0; j < m->columns; ++j) {
            inventory_cleanup(&(m->data[i][j].inv));
        }
    }
    free(m);
}
