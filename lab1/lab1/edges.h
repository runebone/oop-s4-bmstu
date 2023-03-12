#ifndef EDGES_H
#define EDGES_H

#include <cstdio>

struct edge
{
    int point_index_1;
    int point_index_2;
};
using edge_t = struct edge;


struct edge_array
{
    int size;
    edge_t *array;
};
using edge_array_t = struct edge_array;


using edges_t = edge_array_t;

void init_edges(edges_t &edges);
void free_edges(edges_t &edges);

#endif // EDGES_H
