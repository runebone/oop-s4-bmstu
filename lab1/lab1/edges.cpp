#include "edges.h"

#include <cstdlib>

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
