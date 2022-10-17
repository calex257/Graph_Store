#ifndef ALGOS_H
#define ALGOS_H

#include <stdint.h>

/*
*structura pentru a retine o stare din
*parcurgere de la task-ul 3
*/
typedef struct node_state{
    //numarul nodului
    int node_nr;

    //masca
    uint32_t mask;

    //costul pentru a ajunge in starea aceasta
    double cost;
} node_state;

/*
*structura pentru a retine componentele conexe
*si numarul lor
*/
typedef struct CTCList{
    //vector de bitmask-uri, fiecare reprezentand cate o componenta
    uint32_t* components;

    //numarul de componente
    int nr_components;
}CTCList;

double** RoyFloyd(graph* gr);
double Dijkstra_vec(graph* gr, int from, int to, int* parents);
double* Bellman_Ford(graph* gr, int from);
CTCList* Kosaraju(graph* gr, int from);
void sort_CTCList(CTCList* list);
void disp_CTCs(CTCList* list);
double TSP_modif(graph* gr, int from, int to, int included_nodes);

#endif