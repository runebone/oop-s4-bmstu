#include "points.h"

#include <cstdlib> // -> free, malloc
#include <cmath> // -> M_PI, trigonometric funcs

#define POINT_TYPE_IN_FILE 'v'
#define BUFFER_SIZE 256
#define DEG_TO_RAD (M_PI / 180)

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
    if (opened_file == NULL)
    {
        return ERR_NULL_FILE;
    }

    char buffer[BUFFER_SIZE];

    if (fgets(buffer, sizeof(buffer), opened_file) == NULL)
    {
        return ERR_FGETS;
    }

    char type = 0;
    double x, y, z;

    int assigned_count = sscanf(buffer, "%c %lf %lf %lf", &type, &x, &y, &z);

    if (type != POINT_TYPE_IN_FILE)
    {
        return ERR_READ_POINT_INVALID_TYPE;
    }
    else if (assigned_count != 4)
    {
        return ERR_READ_POINT_INVALID_DATA;
    }
    else
    {
        point.x = x;
        point.y = y;
        point.z = z;
    }

    return OK;
}

static err_t count_points(int &n, FILE *opened_file)
{
    if (opened_file == NULL)
    {
        return ERR_NULL_FILE;
    }

    // long int offset = ftell(opened_file);
    err_t error_code = OK;
    int count = 0;
    point_t tmp_point;

    while (error_code == OK)
    {
        error_code = read_point(tmp_point, opened_file);
        count++;
    }
    count--;

    // fseek(opened_file, offset, SEEK_SET);

    if (count > 0)
    {
        error_code = OK;
        n = count;
    }

    return error_code;
}

static err_t read_point_array(point_array_t &point_array, FILE *opened_file)
{
    err_t error_code = OK;
    int n = 0;

    if (opened_file == NULL)
    {
        return ERR_NULL_FILE;
    }

    error_code = count_points(n, opened_file);

    if (error_code == OK)
    {
        point_array.size = n;
        point_array.array = (point_t*)malloc(n * sizeof(point_t));

        if (point_array.array == NULL)
        {
            // Should I also return from here?
            // return ERR_MALLOC;
            error_code = ERR_MALLOC;
        }

        for (int i = 0; error_code == OK && i < n; i++)
        {
            error_code = read_point(point_array.array[i], opened_file);
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
    if (opened_file == NULL)
    {
        return ERR_NULL_FILE;
    }

    char buffer[BUFFER_SIZE];

    sprintf(buffer, "%c %lf %lf %lf\n", POINT_TYPE_IN_FILE, point.x, point.y, point.z);
    fprintf(opened_file, "%s", buffer);

    return OK;
}

static err_t write_point_array(const point_array_t &point_array, FILE *opened_file)
{
    if (opened_file == NULL)
    {
        return ERR_NULL_FILE;
    }

    if (point_array.array == NULL)
    {
        return ERR_NULL_POINT_ARRAY;
    }

    err_t error_code = OK;

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
    if (point_array.array == NULL)
    {
        return ERR_NULL_POINT_ARRAY;
    }

    return OK;
}

err_t points_exist(const points_t &points)
{
    err_t error_code = point_array_exist(points);

    return error_code;
}

static err_t validate_point_array_index(const point_array_t &point_array, int index)
{
    if (index < 0 || index >= point_array.size)
    {
        return ERR_INVALID_INDEX;
    }

    return OK;
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

static err_t find_geometric_center_from_point_array(point_t &center, const point_array_t &point_array)
{
    err_t error_code = point_array_exist(point_array);
    point_t tmp;
    point_t sum;

    for (int i = 0; error_code == OK && i < point_array.size; i++)
    {
        error_code = get_point_by_index_from_points_array(tmp, i, point_array);

        if (error_code == OK)
        {
            sum.x += tmp.x;
            sum.y += tmp.y;
            sum.z += tmp.z;
        }
    }

    if (error_code == OK)
    {
        sum.x /= point_array.size;
        sum.y /= point_array.size;
        sum.z /= point_array.size;

        center = sum;
    }

    return error_code;
}

err_t find_geometric_center(point_t &center, const points_t &points)
{
    err_t error_code = find_geometric_center_from_point_array(center, points);

    return error_code;
}

static void move_point_to_center(point_t &point, const point_t &center)
{
    point.x += center.x;
    point.y += center.y;
    point.z += center.z;
}

static void move_point_from_center(point_t &point, const point_t &center)
{
    point.x -= center.x;
    point.y -= center.y;
    point.z -= center.z;
}

static err_t move_point_array_to_center(point_array_t &point_array, const point_t &center)
{
    err_t error_code = point_array_exist(point_array);

    if (error_code == OK)
    {
        for (int i = 0; i < point_array.size; i++)
        {
            move_point_to_center(point_array.array[i], center);
        }
    }

    return error_code;
}

err_t move_points_to_center(points_t &points, const point_t &center)
{
    err_t error_code = move_point_array_to_center(points, center);

    return error_code;
}

static err_t translate_point_array(point_array_t &point_array, const point_t &translate)
{
    err_t error_code = move_point_array_to_center(point_array, translate); // HACK

    return error_code;
}

err_t translate_points(points_t &points, const point_t &translate)
{
    err_t error_code = translate_point_array(points, translate);

    return error_code;
}

static double to_radians(double degrees)
{
    double radians = degrees * DEG_TO_RAD;

    return radians;
}

static void rotate_x(point_t &point, double degrees)
{
    double theta = to_radians(degrees);
    double y = point.y;
    double z = point.z;

    point.y = y * cos(theta) + z * sin(theta);
    point.z = -y * sin(theta) + z * cos(theta);
}

static void rotate_y(point_t &point, double degrees)
{
    double theta = to_radians(degrees);
    double x = point.x;
    double z = point.z;

    point.x = x * cos(theta) - z * sin(theta);
    point.z = x * sin(theta) + z * cos(theta);
}

static void rotate_z(point_t &point, double degrees)
{
    double theta = to_radians(degrees);
    double x = point.x;
    double y = point.y;

    point.x = x * cos(theta) + y * sin(theta);
    point.y = -x * sin(theta) + y * cos(theta);
}

static void rotate_point(point_t &point, const point_t &rotate_degrees)
{
    rotate_x(point, rotate_degrees.x);
    rotate_y(point, rotate_degrees.y);
    rotate_z(point, rotate_degrees.z);
}

static err_t rotate_point_array(point_array_t &point_array, const point_t &anchor, const point_t &rotate_degrees)
{
    err_t error_code = point_array_exist(point_array);

    if (error_code == OK)
    {
        for (int i = 0; i < point_array.size; i++)
        {
            move_point_from_center(point_array.array[i], anchor);
            rotate_point(point_array.array[i], rotate_degrees);
            move_point_to_center(point_array.array[i], anchor);
        }
    }

    return error_code;
}

err_t rotate_points(points_t &points, const point_t &anchor, const point_t &rotate_degrees)
{
    err_t error_code = rotate_point_array(points, anchor, rotate_degrees);

    return error_code;
}

static void scale_point(point_t &point, const point_t &scale)
{
    point.x *= scale.x;
    point.y *= scale.y;
    point.z *= scale.z;
}

static err_t scale_point_array(point_array_t &point_array, const point_t &anchor, const point_t &scale)
{
    err_t error_code = point_array_exist(point_array);

    if (error_code == OK)
    {
        for (int i = 0; i < point_array.size; i++)
        {
            move_point_to_center(point_array.array[i], anchor);
            scale_point(point_array.array[i], scale);
            move_point_from_center(point_array.array[i], anchor);
        }
    }

    return error_code;
}

err_t scale_points(points_t &points, const point_t &anchor, const point_t &scale)
{
    err_t error_code = scale_point_array(points, anchor, scale);

    return error_code;
}
