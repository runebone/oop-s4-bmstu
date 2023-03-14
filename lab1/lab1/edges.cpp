#include "edges.h"

#include <cstdlib> // -> free, malloc

#define EDGE_TYPE_IN_FILE 'e'
#define BUFFER_SIZE 256

static void init_edge_array(edge_array_t &edge_array)
{
    edge_array.size = 0;
    edge_array.array = NULL;
}

void init_edges(edges_t &edges)
{
    init_edge_array(edges);
}

static void free_edge_array(edge_array_t &edge_array)
{
    if (edge_array.array != NULL)
    {
        free(edge_array.array);
    }
}

void free_edges(edges_t &edges)
{
    free_edge_array(edges);
}

static err_t read_edge(edge_t &edge, FILE *opened_file)
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
        int point_index_1;
        int point_index_2;

        int assigned_count = sscanf(buffer, "%c %d %d", &type, &point_index_1, &point_index_2);

        if (type != EDGE_TYPE_IN_FILE)
        {
            error_code = ERR_READ_EDGE_INVALID_TYPE;
        }
        else if (assigned_count != 3)
        {
            error_code = ERR_READ_EDGE_INVALID_DATA;
        }
        else
        {
            edge.point_index_1 = point_index_1;
            edge.point_index_2 = point_index_2;
        }
    }

    return error_code;
}

static int count_edges(FILE *opened_file)
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

        edge_t tmp_edge;

        while (error_code == OK)
        {
            error_code = read_edge(tmp_edge, opened_file);
            count++;
        }
        count--;

        fseek(opened_file, offset, SEEK_SET);
    }

    return count;
}

static err_t read_edges_array(edge_array_t &edge_array, FILE *opened_file)
{
    err_t error_code = OK;

    if (opened_file == NULL)
    {
        error_code = ERR_NULL_FILE;
    }

    if (error_code == OK)
    {
        int n = count_edges(opened_file);

        edge_array.size = n;
        edge_array.array = (edge_t*)malloc(n * sizeof(edge_t));

        if (edge_array.array == NULL)
        {
            error_code = ERR_MALLOC;
        }

        for (int i = 0; error_code == OK && i < n; i++)
        {
            read_edge(edge_array.array[i], opened_file);
        }
    }

    return error_code;
}

err_t read_edges(edges_t &edges, FILE *opened_file)
{
    err_t error_code = read_edges_array(edges, opened_file);

    return error_code;
}


static err_t write_edge(const edge_t &edge, FILE *opened_file)
{
    err_t error_code = OK;

    if (opened_file == NULL)
    {
        error_code = ERR_NULL_FILE;
    }

    if (error_code == OK)
    {
        char buffer[BUFFER_SIZE];

        sprintf(buffer, "%c %d %d\n", EDGE_TYPE_IN_FILE, edge.point_index_1, edge.point_index_2);

        fprintf(opened_file, "%s", buffer);
    }

    return error_code;
}

static err_t write_edge_array(const edge_array_t &edge_array, FILE *opened_file)
{
    err_t error_code = OK;

    if (opened_file == NULL)
    {
        error_code = ERR_NULL_FILE;
    }

    if (error_code == OK)
    {
        if (edge_array.array == NULL)
        {
            error_code = ERR_NULL_EDGE_ARRAY;
        }
    }

    for (int i = 0; error_code == OK && i < edge_array.size; i++)
    {
        error_code = write_edge(edge_array.array[i], opened_file);
    }

    return error_code;
}

err_t write_edges(const edges_t &edges, FILE *opened_file)
{
    err_t error_code = write_edge_array(edges, opened_file);

    return error_code;
}

static err_t edge_array_exist(const edge_array_t &edge_array)
{
    err_t error_code = OK;

    if (edge_array.array == NULL)
    {
        error_code = ERR_NULL_EDGE_ARRAY;
    }

    return error_code;
}

err_t edges_exist(const edges_t &edges)
{
    err_t error_code = edge_array_exist(edges);

    return error_code;
}

static err_t validate_edge_array_index(const edge_array_t &edge_array, int index)
{
    err_t error_code = OK;

    if (index < 0 || index >= edge_array.size)
    {
        error_code = ERR_INVALID_INDEX;
    }

    return error_code;
}

static err_t get_edge_by_index_from_edge_array(edge_t &edge, int index, const edge_array_t &edge_array)
{
    err_t error_code = validate_edge_array_index(edge_array, index);

    if (error_code == OK)
    {
        edge = edge_array.array[index];
    }

    return error_code;
}

err_t get_edge_by_index(edge_t &edge, int index, const edges_t &edges)
{
    err_t error_code = get_edge_by_index_from_edge_array(edge, index, edges);

    return error_code;
}
