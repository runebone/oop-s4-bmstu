#include "points.h"

#include <cstdlib> // -> free, malloc

#define POINT_TYPE_IN_FILE 'v'
#define BUFFER_SIZE 256

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

static err_t read_point(point_t &point, FILE *opened_file)
{
    err_t error_code = OK;
    char buffer[BUFFER_SIZE];

    if (opened_file == NULL)
    {
        error_code = ERR_NULL_FILE;
    }

    if (error_code == OK)
    {
        if (fgets(buffer, sizeof(buffer), opened_file) == NULL)
        {
            error_code = ERR_FGETS;
        }
    }

    if (error_code == OK)
    {
        char type = 0;
        double x;
        double y;
        double z;

        int assigned_count = sscanf(buffer, "%c %lf %lf %lf", &type, &x, &y, &z);

        if (type != POINT_TYPE_IN_FILE)
        {
            error_code = ERR_READ_POINT_INVALID_TYPE;
        }
        else if (assigned_count != 4)
        {
            error_code = ERR_READ_POINT_INVALID_DATA;
        }
        else
        {
            point.x = x;
            point.y = y;
            point.z = z;
        }
    }

    return error_code;
}

static int count_points(FILE *opened_file)
{
    err_t error_code = OK;
    int count = 0;

    if (opened_file == NULL)
    {
        error_code = ERR_NULL_FILE;
    }

    if (error_code == OK)
    {
        long int offset = ftell(opened_file);

        point_t tmp_point;

        while (error_code == OK)
        {
            error_code = read_point(tmp_point, opened_file);
            count++;
        }
        count--;

        fseek(opened_file, offset, SEEK_SET);
    }

    return count;
}

static err_t read_point_array(point_array_t &point_array, FILE *opened_file)
{
    err_t error_code = OK;

    if (opened_file == NULL)
    {
        error_code = ERR_NULL_FILE;
    }

    if (error_code == OK)
    {
        int n = count_points(opened_file);

        point_array.size = n;
        point_array.array = (point_t*)malloc(n * sizeof(point_t));

        for (int i = 0; i < n; i++)
        {
            read_point(point_array.array[i], opened_file);
        }
    }

    return error_code;
}

err_t read_points(points_t &points, FILE *opened_file)
{
    err_t error_code = read_point_array(points, opened_file);

    return error_code;
}

static err_t write_point(const point_t &point, FILE *opened_file)
{
    err_t error_code = OK;

    if (opened_file == NULL)
    {
        error_code = ERR_NULL_FILE;
    }

    if (error_code == OK)
    {
        char buffer[BUFFER_SIZE];

        sprintf(buffer, "%c %lf %lf %lf\n", POINT_TYPE_IN_FILE, point.x, point.y, point.z);

        fprintf(opened_file, "%s", buffer);
    }

    return error_code;
}

static err_t write_point_array(const point_array_t &point_array, FILE *opened_file)
{
    err_t error_code = OK;

    if (opened_file == NULL)
    {
        error_code = ERR_NULL_FILE;
    }

    if (error_code == OK)
    {
        if (point_array.array == NULL)
        {
            error_code = ERR_NULL_POINT_ARRAY;
        }
    }

    for (int i = 0; error_code == OK && i < point_array.size; i++)
    {
        error_code = write_point(point_array.array[i], opened_file);
    }

    return error_code;
}

err_t write_points(const points_t &points, FILE *opened_file)
{
    err_t error_code = write_point_array(points, opened_file);

    return error_code;
}

static err_t point_array_exist(const point_array_t &point_array)
{
    err_t error_code = OK;

    if (point_array.array == NULL)
    {
        error_code = ERR_NULL_POINT_ARRAY;
    }

    return error_code;
}

err_t points_exist(const points_t &points)
{
    err_t error_code = point_array_exist(points);

    return error_code;
}

static err_t validate_point_array_index(const point_array_t &point_array, int index)
{
    err_t error_code = OK;

    if (index < 0 || index >= point_array.size)
    {
        error_code = ERR_INVALID_INDEX;
    }

    return error_code;
}

static err_t get_point_by_index_from_points_array(point_t &point, int index, const point_array_t &point_array)
{
    err_t error_code = validate_point_array_index(point_array, index);

    if (error_code == OK)
    {
        point = point_array.array[index];
    }

    return error_code;
}

err_t get_point_by_index(point_t &point, int index, const points_t &points)
{
    err_t error_code = get_point_by_index_from_points_array(point, index, points);

    return error_code;
}
