#include "edges.h"

static void init_edge_array(edge_array_t &edge_array)
{
    edge_array.size = 0;
    edge_array.array = NULL;
}

void init_edges(edges_t &edges)
{
    init_edge_array(edges);
}
