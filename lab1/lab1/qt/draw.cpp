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

static err_t scene_exists(const QGraphicsScene *scene)
{
    if (scene == nullptr)
    {
        return ERR_NULL_SCENE;
    }

    return OK;
}

err_t canvas_exists(const canvas_t &canvas)
{
    err_t error_code = scene_exists(canvas.scene);

    return error_code;
}

static void clear_scene(QGraphicsScene *scene)
{
    scene->clear();
}

void clear_canvas(const canvas_t &canvas)
{
    clear_scene(canvas.scene);
}

static QPen get_default_pen()
{
    QPen pen = QPen(Qt::black, 2);

    return pen;
}

static void qt_draw_line_with_pen(QGraphicsScene *scene, const point_t &point_1, const point_t &point_2, const QPen &pen)
{
    scene->addLine(point_1.x, point_1.y, point_2.x, point_2.y, pen);
}

static void qt_draw_line(QGraphicsScene *scene, const point_t &point_1, const point_t &point_2)
{
    QPen pen = get_default_pen();

    qt_draw_line_with_pen(scene, point_1, point_2, pen);
}

static void center_point(point_t &point, const canvas_t &canvas)
{
    // XXX: this requires canvas.scene to have fixed width and height to work properly
    double width = get_canvas_width(canvas);
    double height = get_canvas_height(canvas);

    point.x += width / 2;
    point.y += height / 2;
}

static void draw_line(const canvas_t &canvas, const point_t &point_1, const point_t &point_2)
{
    point_t centered_point_1 = point_1;
    point_t centered_point_2 = point_2;

    center_point(centered_point_1, canvas);
    center_point(centered_point_2, canvas);

    qt_draw_line(canvas.scene, centered_point_1, centered_point_2);
}

static err_t draw_projection(const canvas_t &canvas, const points_t &points, const edges_t &edges)
{
    err_t error_code = OK;

    int n = get_edges_size(edges);
    point_t point_1;
    point_t point_2;
    edge_t edge;

    // V1; initial version
    for (int i = 0; error_code == OK && i < n; i++)
    {
        error_code = get_edge_by_index(edge, i, edges);

        if (error_code == OK)
        {
            error_code = get_point_by_index(point_1, edge.point_index_1, points);
        }

        if (error_code == OK)
        {
            error_code = get_point_by_index(point_2, edge.point_index_2, points);
        }

        if (error_code == OK)
        {
            draw_line(canvas, point_1, point_2);
        }
    }

    // V2; nest if statements; avoid potential extra checks
    for (int i = 0; error_code == OK && i < n; i++)
    {
        error_code = get_edge_by_index(edge, i, edges);

        if (error_code == OK)
        {
            error_code = get_point_by_index(point_1, edge.point_index_1, points);

            if (error_code == OK)
            {
                error_code = get_point_by_index(point_2, edge.point_index_2, points);

                if (error_code == OK)
                {
                    draw_line(canvas, point_1, point_2);
                }
            }
        }
    }

    // V3; avoid both nesting and potential extra checks; but use "break"-s
    for (int i = 0; error_code == OK && i < n; i++)
    {
        error_code = get_edge_by_index(edge, i, edges);

        if (error_code != OK) break;

        error_code = get_point_by_index(point_1, edge.point_index_1, points);

        if (error_code != OK) break;

        error_code = get_point_by_index(point_2, edge.point_index_2, points);

        if (error_code != OK) break;

        draw_line(canvas, point_1, point_2);
    }

    return error_code;
}


err_t draw_wireframe(const canvas_t &canvas, const points_t &points, const edges_t &edges)
{
    err_t error_code = draw_projection(canvas, points, edges);

    return error_code;
}
