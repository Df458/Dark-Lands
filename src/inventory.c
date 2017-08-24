#include "compat.h"
#include "inventory.h"
#include <stdlib.h>
#include <string.h>

void give_item(inventory* inv, item* it) {
    // Check for an identical item. If it exists, combine stacks and return.
    for(int i = 0; i < inv->item_count; ++i) {
        if(!strcmp(inv->item_list[i]->name, it->name)) {
            inv->item_list[i]->count += it->count;
            free(it);
            return;
        }
    }

    // If there's no more room for items, resize the container
    if(inv->item_count >= inv->item_slots) {
        inv->item_slots *= 2;
        inv->item_list = realloc(inv->item_list, inv->item_slots * sizeof(item*));
    }

    // Place the item in the list
    inv->item_list[inv->item_count] = it;
    inv->item_count++;
}
item* take_item(inventory* inv, uint16_t index) {
    if(inv->item_count > index) {
        item* it = inv->item_list[index];

        // Shift the items backwards. There are faster methods of filling
        // space, but we want to preserve the list's order for the player's sake.
        for(int i = index + 1; i < inv->item_count; ++i)
            inv->item_list[i - 1] = inv->item_list[i];

        inv->item_count--;

        return it;
    }

    return NULL;
}
void item_run_callback(inventory* inv, uint16_t index, actor* a, item_callback c) {
    if(inv->item_count > index && c != NULL) {
        if(c(inv->item_list[index], a)) {
            take_item(inv, index);
        }
    }
}

int32_t inventory_panel(uint8_t can_default, inventory* inv) {
    if(can_default) {
        if(inv->item_count == 0)
            return -1;
        else if(inv->item_count == 1)
            return 0;
    }
    int32_t it = -1;
    uint8_t done = FALSE;
    WINDOW* inventory_window = newwin(25, 40, 0, 40);
    PANEL* inventory_panel = new_panel(inventory_window);
    uint16_t scroll_position = 0;
    uint16_t cursor_position = 0;

    top_panel(inventory_panel);

    while(!done) {
        wclear(inventory_window);
        box(inventory_window, 0, 0);

        mvwaddch(inventory_window, 22, 0, 195);
        for(int i = 1; i < 39; ++i)
            mvwaddch(inventory_window, 22, i, 196);
        mvwaddch(inventory_window, 22, 39, 180);
        mvwaddstr(inventory_window, 23, 1, "ENTER to select, q to exit");

        if(scroll_position > 0)
            mvwaddstr(inventory_window, 0, 16, "^ More ^");
        if(scroll_position + 21 < inv->item_count)
            mvwaddstr(inventory_window, 22, 16, "v More v");

        for(int i = scroll_position; i < inv->item_count && i < scroll_position + 21; ++i) {
            if(i == cursor_position)
                wattron(inventory_window, A_STANDOUT);
            mvwprintw(inventory_window, i + 1 - scroll_position, 1, "%2d %s", inv->item_list[i]->count, inv->item_list[i]->name);
            if(i == cursor_position)
                wattroff(inventory_window, A_STANDOUT);
        }

        update_panels();
        doupdate();

        switch(getch()) {
            case 'q':
                done = TRUE;
                break;
            case '\n':
                it = cursor_position;
                done = TRUE;
                break;
            case KEY_UP:
                if(cursor_position > 0) {
                    cursor_position--;
                    if(cursor_position < scroll_position)
                        scroll_position--;
                }
                break;
            case KEY_DOWN:
                if(cursor_position < inv->item_count - 1) {
                    cursor_position++;
                    if(cursor_position >= scroll_position + 21)
                        scroll_position++;
                }
                break;
        }
    }

    del_panel(inventory_panel);
    delwin(inventory_window);

    return it;
}
void inventory_cleanup(inventory* inv) {
    for(int i = 0; i < inv->item_count; ++i)
        item_free(inv->item_list[i]);
    free(inv->item_list);
}
