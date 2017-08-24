#ifndef INVENTORY_H
#define INVENTORY_H
#include "item.h"

typedef struct inventory {
    uint16_t item_count;
    uint16_t item_slots;
    item** item_list;
} inventory;

void give_item(inventory* inv, item* i);
item* take_item(inventory* inv, uint16_t index);

int32_t inventory_panel(uint8_t select, inventory* inv);
void item_run_callback(inventory* inv, uint16_t index, actor* a, item_callback c);
void inventory_cleanup(inventory* inv);

#endif
