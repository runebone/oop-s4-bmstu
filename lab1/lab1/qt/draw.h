#ifndef DRAW_H
#define DRAW_H

#include <QGraphicsScene>
#include "errors.h"

struct canvas
{
    QGraphicsScene *scene = nullptr;
};
using canvas_t = struct canvas;

err_t canvas_exists(const canvas_t &canvas);
void clear_canvas(const canvas_t &canvas);

#endif // DRAW_H
