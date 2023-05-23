#include "figure.h"

figure_t &init_figure()
{
    static figure_t figure;

    init_points(figure.points);
    init_edges(figure.edges);
    set_to_zero(figure.center);

    return figure;
}

static err_t read_figure(figure_t &figure, FILE *opened_file)
{
    if (opened_file == NULL)
    {
        return ERR_NULL_FILE;
    }

    err_t error_code = read_points(figure.points, opened_file);

    if (error_code == OK)
    {
        error_code = read_edges(figure.edges, opened_file);

        if (error_code != OK)
        {
            free_points(figure.points);
        }
    }

    return error_code;
}

err_t load_figure(figure_t &figure, const filename_t &filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return ERR_OPEN_FILE;
    }

    figure_t tmp_figure;
    err_t error_code = read_figure(tmp_figure, file);
    fclose(file);

    if (error_code == OK)
    {
        free_figure(figure);
        figure = tmp_figure;
    }

    return error_code;
}

static err_t write_figure(const figure_t &figure, FILE *opened_file)
{
    if (opened_file == NULL)
    {
        return ERR_NULL_FILE;
    }

    err_t error_code = write_points(figure.points, opened_file);

    if (error_code == OK)
    {
        error_code = write_edges(figure.edges, opened_file);
    }

    return error_code;
}

err_t save_figure(const figure_t &figure, const filename_t &filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        return ERR_OPEN_FILE;
    }

    err_t error_code = write_figure(figure, file);
    fclose(file);

    return error_code;
}

static err_t figure_exists(const figure_t &figure)
{
    err_t error_code = points_exist(figure.points);

    if (error_code == OK)
    {
        error_code = edges_exist(figure.edges);
    }

    return error_code;
}


err_t draw_figure(const figure_t &figure, const canvas_t &canvas)
{
    err_t error_code = figure_exists(figure);

    if (error_code == OK)
    {
        error_code = canvas_exists(canvas);

        if (error_code == OK)
        {
            clear_canvas(canvas);
            error_code = draw_wireframe(canvas, figure.points, figure.edges);
        }
    }

    return error_code;
}


err_t translate_figure(figure_t &figure, const change_t &change)
{
    err_t error_code = translate_points(figure.points, change);

    return error_code;
}

err_t rotate_figure(figure_t &figure, const change_t &change)
{
    err_t error_code = rotate_points(figure.points, figure.center, change);

    return error_code;
}

err_t scale_figure(figure_t &figure, const change_t &change)
{
    err_t error_code = scale_points(figure.points, figure.center, change);

    return error_code;
}


void free_figure(figure_t &figure)
{
    free_points(figure.points);
    free_edges(figure.edges);
}
