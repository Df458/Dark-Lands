#include "actor.h"
#include "compat.h"
#include "item.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

uint8_t trap_step(item* i, actor* a) {
    a->hp -= 5;
    log_write("You step on a %s, and lose 5 hp", i->name);
    free(i);
    return TRUE;
}
uint8_t potion_use(item* i, actor* a) {
    a->hp += 5;
    log_write("You drink the %s, and heal 5 hp", i->name);
    free(i);
    return TRUE;
}

const item i_boots = {
    .name = "boots",
    .character = 'b',
    .color_pair = 0
};
const item i_trap = {
    .name = "trap",
    .character = 'X',
    .color_pair = 0,
    .on_step = trap_step
};
const item i_potion = {
    .name = "potion",
    .character = '!',
    .color_pair = 1,
    .on_use = potion_use
};

item* create_boots(uint16_t count) {
    item* i = malloc(sizeof(item));
    i = memcpy(i, &i_boots, sizeof(item));
    i->name = strdup(i_boots.name);
    i->count = count;
    return i;
}

item* create_trap(uint16_t count) {
    item* i = malloc(sizeof(item));
    i = memcpy(i, &i_trap, sizeof(item));
    i->name = strdup(i_trap.name);
    i->count = count;
    return i;
}

item* create_potion(uint16_t count) {
    item* i = malloc(sizeof(item));
    i = memcpy(i, &i_potion, sizeof(item));
    i->name = strdup(i_potion.name);
    i->count = count;
    return i;
}

void item_free(item* it) {
    free(it->name);
    free(it);
}
