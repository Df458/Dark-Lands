#ifndef DRAW_H
#define DRAW_H
#include <curses.h>

enum alignment
{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};

/*
 * This function lets the user draw a string to a specified row, with
 * automatic alignment. If the chosen row is outside the bounds, or the text is
 * too wide, the function draws nothing and returns.
 *
 * align specifies the alignment of the text, and expects an alignment value as
 * declared above.
 */
void draw_text_aligned(WINDOW* win, int row, const char* text, int align);

/*
 * This function draws a bunch of characters onto the screen to simulate stars.
 */
void draw_stars(WINDOW* win);

/*
 * This function draws a filled rectangle with the specified dimensions at the
 * specified position. The shape argument determines what character will be the
 * fill for the rectangle.
 *
 * If the rectangle would go out of bounds, it is cut off.
 */
void draw_rect(WINDOW* win, int row, int col, int height, int width, char shape);

#endif
