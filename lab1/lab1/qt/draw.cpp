#include "draw.h"

static double get_scene_width(const QGraphicsScene *scene)
{
    double width = scene->width();

    return width;
}

static double get_scene_height(const QGraphicsScene *scene)
{
    double height = scene->height();

    return height;
}

static double get_canvas_width(const canvas_t &canvas)
{
    double width = get_scene_width(canvas.scene);

    return width;
}

static double get_canvas_height(const canvas_t &canvas)
{
    double height = get_scene_height(canvas.scene);

    return height;
}
