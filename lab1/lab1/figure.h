#ifndef FIGURE_H
#define FIGURE_H

#include "qt/draw.h" // -> canvas_t
#include "errors.h"
#include "points.h"
#include "edges.h"

struct figure
{
    points_t points;
    edges_t edges;
    point_t center;
};
using figure_t = struct figure;


using filename_t = const char *;


using change_t = point_t;


figure_t &init_figure(void);

err_t load_figure(figure_t &figure, const filename_t &filename);
err_t save_figure(const figure_t &figure, const filename_t &filename);

err_t draw_figure(const figure_t &figure, const canvas_t &canvas);

err_t translate_figure(figure_t &figure, const change_t &change);
err_t rotate_figure(figure_t &figure, const change_t &change);
err_t scale_figure(figure_t &figure, const change_t &change);

void free_figure(figure_t &figure);

#endif // FIGURE_H
