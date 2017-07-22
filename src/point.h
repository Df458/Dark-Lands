#ifndef POINT_H
#define POINT_H
#include <inttypes.h>

typedef struct point {
    uint16_t line;
    uint16_t column;
} point;
point point_add(point p1, point p2);
point point_sub(point p1, point p2);

#endif
