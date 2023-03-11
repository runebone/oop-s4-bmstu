#ifndef EDGES_H
#define EDGES_H

#include <cstdio>

struct edge {
    int point_index_1;
    int point_index_2;
};
using edge_t = struct edge;


struct edge_array {
    int size = 0;
    edge_t *array = NULL;
};
using edge_array_t = struct edge_array;


using edges_t = edge_array_t;

#endif // EDGES_H
