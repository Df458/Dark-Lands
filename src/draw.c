#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "draw.h"
#include "math_util.h"

/*
 * This function lets the user draw a string to a specified row, with
 * automatic alignment. If the chosen row is outside the bounds, or the text is
 * too wide, the function draws nothing and returns.
 *
 * align specifies the alignment of the text, and expects an alignment value as
 * declared above.
 */
void draw_text_aligned(WINDOW* win, int row, const char* text, int align)
{
    int rows, cols;
    getmaxyx(win, rows, cols);

    // If we're out of bounds, or we don't have enough space, return
    if(row > rows || strlen(text) > cols)
        return;

    switch(align) {
        case ALIGN_LEFT:
            mvwaddstr(win, row, 0, text);
            break;
        case ALIGN_CENTER:
            mvwaddstr(win, row, (cols / 2) - (strlen(text) / 2), text);
            break;
        case ALIGN_RIGHT:
            mvwaddstr(win, row, cols - strlen(text), text);
            break;
    }
}

/*
 * This function draws a bunch of characters onto the screen to simulate stars.
 */
void draw_stars(WINDOW* win)
{
    int rows, cols;
    getmaxyx(win, rows, cols);

    char star_chars[] = { '.', '.', '.', '*', '+' };

    int star_count = rand() % 100 + 50;
    for(int i = 0; i < star_count; ++i) {
        mvwaddch(win, rand() % rows, rand() % cols, star_chars[rand() % 5]);
    }
}

/*
 * This function draws a filled rectangle with the specified dimensions at the
 * specified position. The shape argument determines what character will be the
 * fill for the rectangle.
 *
 * If the rectangle would go out of bounds, it is cut off.
 */
void draw_rect(WINDOW* win, int row, int col, int height, int width, char shape, int filled)
{
    int rows, cols;
    getmaxyx(win, rows, cols);

    int imin = clamp(row, 0, rows);
    int imax = clamp(row + height, 0, rows);
    int jmin = clamp(col, 0, cols);
    int jmax = clamp(col + width, 0, cols);
    for(int i = imin; i < imax; ++i) {
        for(int j = jmin; j < jmax; ++j) {
            char s = shape;
            if(!filled && j != jmin && i != imin && j != jmax - 1 && i != imax - 1)
                s = ' ';
            mvwaddch(win, i, j, s);
        }
    }
}

/*
 * This function draws a filled ellipse with the specified dimensions at the
 * specified position. The shape argument determines what character will be the
 * fill for the rectangle.
 *
 * If the ellipse would go out of bounds, it is cut off.
 */
void draw_ellipse(WINDOW* win, int row, int col, int height, int width, char shape)
{
    int rows, cols;
    getmaxyx(win, rows, cols);

    for(int i = clamp(row, 0, rows); i < clamp(row + height, 0, rows); ++i) {
        float ratio = (float)(i - row) / height;
        float w = sin(degtorad(ratio * 180)) * width;
        for(int j = clamp(col + ((width - ceil(w))), 0, cols); j < clamp(col + ceil(w), 0, cols); ++j) {
            mvwaddch(win, i, j, shape);
        }
    }
}
