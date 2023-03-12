#include "figure.h"

figure_t &init_figure()
{
    static figure_t figure;

    init_points(figure.points);
    init_edges(figure.edges);
    set_to_zero(figure.center);

    return figure;
}


err_t load_figure(figure_t &figure, const filename_t &filename)
{
    err_t error_code = OK;

    return error_code;
}

err_t save_figure(const figure_t &figure, const filename_t &filename)
{
    err_t error_code = OK;

    return error_code;
}


err_t draw_figure(const figure_t &figure, const canvas_t &canvas)
{
    err_t error_code = OK;

    return error_code;
}


err_t translate_figure(figure_t &figure, const change_t &change)
{
    err_t error_code = OK;

    return error_code;
}

err_t rotate_figure(figure_t &figure, const change_t &change)
{
    err_t error_code = OK;

    return error_code;
}

err_t scale_figure(figure_t &figure, const change_t &change)
{
    err_t error_code = OK;

    return error_code;
}


void free_figure(figure_t &figure)
{
    free_points(figure.points);
    free_edges(figure.edges);
}
