#ifndef ITEM_H
#define ITEM_H
#include <inttypes.h>

typedef struct item item;
typedef struct actor actor;

typedef uint8_t (*item_callback)(item* i, struct actor* target);

typedef struct item {
    char* name;
    uint8_t character;
    int color_pair;

    uint16_t count;

    item_callback on_use;
    item_callback on_step;
} item;

typedef struct actor actor;

item* create_boots(uint16_t count);
item* create_trap(uint16_t count);
item* create_potion(uint16_t count);

void item_free(item* it);

#endif
