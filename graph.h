#ifndef GRAPH_C
#define GRAPH_C

#include <stdint.h>
#include "adj_list.h"
#include "stack.h"


typedef struct graph {
    int nr_nodes, nr_edges;
    struct node** neighbours;
    int nr_deposits;
    uint32_t deposits;
} graph;

void add_edge(graph *gr, int from, int to, double cost);
void init_adj_list(graph* gr);
void read_graph(graph *gr, FILE* file);
void remove_edge(graph *gr, int from, int to);
void display_graph(graph* gr);
void free_graph(graph* gr);
void BFS_graph(graph*gr, int begin_from);
void DFS_graph(graph* gr, int begin_from, uint32_t* visited, Stack* st);
graph* transpose_graph(graph* gr);
void add_to_vis(uint32_t* vis, uint32_t nd);
int is_visited(uint32_t* vis, uint32_t nd);
void elim_from_vis(uint32_t* vis, uint32_t nd);

#endif