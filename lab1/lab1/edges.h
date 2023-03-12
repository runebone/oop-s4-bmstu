#ifndef EDGES_H
#define EDGES_H

#include <cstdio>
#include "errors.h"

struct edge
{
    int point_index_1;
    int point_index_2;
};
using edge_t = struct edge;


struct edge_array
{
    int size = 0;
    edge_t *array = NULL;
};
using edge_array_t = struct edge_array;


using edges_t = edge_array_t;

void init_edges(edges_t &edges);
void free_edges(edges_t &edges);

err_t read_edges(edges_t &edges, FILE *opened_file);
err_t write_edges(const edges_t &edges, FILE *opened_file);

err_t edges_exist(const edges_t &edges);

#endif // EDGES_H
