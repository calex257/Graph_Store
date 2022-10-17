#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "adj_list.h"
#include "queue.h"
#include "graph.h"
#include "stack.h"

void read_graph(graph* gr, FILE* file)
{
    int to, from;
    double cost;
    char buffer[50];
    fgets(buffer, 50, file);
    sscanf(buffer, "%d%d%d", &(gr->nr_nodes), &(gr->nr_edges), &(gr->nr_deposits));
    init_adj_list(gr);
    for(int i=0;i<gr->nr_edges;i++) {
        fgets(buffer, 50, file);
        sscanf(buffer, "%d%d%lf", &from, &to, &cost);
        add_edge(gr, from, to, cost);
    }
    gr->deposits = 0;
    fgets(buffer, 50, file);
    char* p = strtok(buffer, " ");
    for(int i=0;i<gr->nr_deposits;i++){
        int dep;
        sscanf(p, "%d", &dep);
        add_to_vis(&gr->deposits, dep);
        p = strtok(NULL, " ");
    }
}

void init_adj_list(graph* gr)
{
    gr->neighbours = (node**)calloc(gr->nr_nodes, sizeof(node*));
}

void add_edge(graph* gr, int from, int to, double cost)
{
    push_al(&gr->neighbours[from], to, cost);
}

void remove_edge(graph* gr, int from, int to)
{
    free(remove_node(&gr->neighbours[from], to));
}

void print_int(int nr)
{
    printf("%d, ", nr);
}

void display_graph(graph* gr)
{
    for (int i = 0;i < gr->nr_nodes;i++) {
        printf("nodul %d: ", i);
        print_list_al(gr->neighbours[i], print_int);
        puts("");
    }
    puts("Deposits");
    for(int i=0;i<32;i++) {
        if(is_visited(&gr->deposits, i)){
            printf("%d ", i);
        }
    }
    puts("");
}

void free_graph(graph* gr)
{
    for (int i = 0;i < gr->nr_nodes;i++) {
        free_list_al(&gr->neighbours[i]);
    }
    free(gr->neighbours);
    free(gr);
}

void add_to_vis(uint32_t* vis, uint32_t nd)
{
    (*vis)|=(1<<(31-nd));
}

int is_visited(uint32_t* vis, uint32_t nd)
{
    return (((*vis)&(1<<(31-nd)))!=0)?1:0;
}

void elim_from_vis(uint32_t* vis, uint32_t nd)
{
    uint32_t mask = ~(1<<(31-nd));
    (*vis)&=mask;
}

graph* transpose_graph(graph* gr)
{
    graph* transp = malloc(sizeof(graph));
    transp->nr_nodes = gr->nr_nodes;
    init_adj_list(transp);
    for(int i=0;i<gr->nr_nodes;i++){
        node* iter = gr->neighbours[i];
        while(iter){
            add_edge(transp, iter->node_num, i, iter->cost);
            iter=iter->next;
        }
    }
    return transp;
}

void DFS_graph(graph* gr, int begin_from, uint32_t* visited, Stack* st)
{
    if(is_visited(visited, begin_from)){
        return;
    }
    add_to_vis(visited, begin_from);
    node* iter = gr->neighbours[begin_from];
    while(iter!=NULL){
        if(!is_visited(visited, iter->node_num)){
            DFS_graph(gr, iter->node_num, visited, st);
        }
        iter=iter->next;
    }
    if(!is_visited(&gr->deposits, begin_from)){
        add_stack(st, begin_from);
    }
}