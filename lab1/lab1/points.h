#ifndef POINT_H
#define POINT_H

#include <cstdlib>

struct point {
    double x;
    double y;
    double z;
};
using point_t = struct point;


struct point_array {
    int size = 0;
    point_t *array = NULL;
};
using point_array_t = struct point_array;


using points_t = point_array_t;

#endif // POINT_H
