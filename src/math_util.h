#ifndef MATH_UTIL
#define MATH_UTIL

#define PI 3.14159265

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)
#define clamp(a, b, c) (min(max(a, b), c))
#define degtorad(deg) (deg * (PI / 180))
#define radtodeg(rad) (rad * (180 / PI))

#endif
