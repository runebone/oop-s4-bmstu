#ifndef DRAW_H
#define DRAW_H

#include <QGraphicsScene>
#include "errors.h"
#include "points.h"
#include "edges.h"

struct canvas
{
    QGraphicsScene *scene = nullptr;
};
using canvas_t = struct canvas;

err_t canvas_exists(const canvas_t &canvas);
void clear_canvas(const canvas_t &canvas);

err_t draw_wireframe(const canvas_t &canvas, const points_t &points, const edges_t &edges);

#endif // DRAW_H
