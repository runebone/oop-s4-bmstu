#include "points.h"

#include <cstdlib>

static void init_point_array(point_array_t &point_array)
{
    point_array.size = 0;
    point_array.array = NULL;
}

void init_points(points_t &points)
{
    init_point_array(points);
}

void set_to_zero(point_t &point)
{
    point.x = 0.0;
    point.y = 0.0;
    point.z = 0.0;
}

static void free_point_array(point_array_t &point_array)
{
    if (point_array.array != NULL)
    {
        free(point_array.array);
    }
}

void free_points(points_t &points)
{
    free_point_array(points);
}
