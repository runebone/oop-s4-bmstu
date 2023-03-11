#ifndef DRAW_H
#define DRAW_H

#include <QGraphicsScene>

struct canvas {
    QGraphicsScene *scene;

    double width;
    double height;
};
using canvas_t = struct canvas;

#endif // DRAW_H
