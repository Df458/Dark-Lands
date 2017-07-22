#include "point.h"

point point_add(point p1, point p2) {
    return (point){ .line=p1.line + p2.line, .column=p1 . column+p2.column };
}
point point_sub(point p1, point p2) {
    return (point){ .line=p1.line - p2.line, .column=p1.column - p2.column };
}

