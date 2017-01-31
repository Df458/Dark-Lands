#include "animation.h"
#include <math.h>

static float angle = 0.f;
static float inout = 0.f;
static int   colorlist[3] = { 0, 7, 8 };

void animation_draw(WINDOW* win) {
    wclear(win);
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    inout += 0.05f;
    angle += (sin(inout) + 1.f) * 0.025 + 0.05;
    for(int i = 0; i < 10; ++i) {
        wattron(win, COLOR_PAIR(colorlist[i/4]));
        for(int j = 0; j < 5; ++j) {
            int l = (sin(angle + (0.05f * i) - (6.2831852 * (1.f / 5.f) * j)) * (10.f + sin(inout) * 3)) + 20.f + 0.5f;
            int c = (cos(angle + (0.05f * i) - (6.2831852 * (1.f / 5.f) * j)) * (20.f + sin(inout) * 6)) + 40.f + 0.5f;

            mvwaddch(win, l, c, 219);
        }
        wattroff(win, COLOR_PAIR(colorlist[i/4]));
    }
}
