#ifndef POINT_H
#define POINT_H

#include <cstdio>
#include "errors.h"

struct point
{
    double x;
    double y;
    double z;
};
using point_t = struct point;


struct point_array
{
    int size = 0;
    point_t *array = NULL;
};
using point_array_t = struct point_array;


using points_t = point_array_t;

void init_points(points_t &points);
void set_to_zero(point_t &point);
void free_points(points_t &points);

err_t read_points(points_t &points, FILE *opened_file);
err_t write_points(const points_t &points, FILE *opened_file);

err_t points_exist(const points_t &points);

#endif // POINT_H
