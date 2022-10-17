#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "graph.h"
#include "stack.h"
#include "adj_list.h"
#include "gen_queue.h"
#include "algos.h"

#define min(x, y) (x)<(y)?(x):(y)

int state_is_solution(node_state* ref, node_state* cmp)
{
    if(ref->mask == cmp->mask && ref->node_nr == cmp->node_nr){
        return 1;
    }
    return 0;
}

int state_out_of_bounds(uint32_t bounds, uint32_t state)
{
    if((state|bounds)!=bounds){
        return 1;
    }
    return 0;
}

int node_visited(const void* e1, const void* e2)
{
    node_state* s1=(node_state*)e1, *s2=(node_state*)e2;
    if(s1->node_nr == s2->node_nr){
        return 1;
    }
    return 0;
}

int states_are_equal(const void* e1, const void* e2)
{
    node_state* s1=(node_state*)e1, *s2=(node_state*)e2;
    if(s1->mask==s2->mask && s1->node_nr == s2->node_nr && s1->cost>s2->cost){
        return 1;
    }
    return 0;
}

void disp_state(void* e)
{
    node_state* st = (node_state*)e;
    printf("%d %x %lf\n", st->node_nr, st->mask, st->cost);
}

double TSP_modif(graph* gr, int from, int to, int included_nodes)
{
    double min_cost = INT_MAX;
    uint32_t bounds = included_nodes | gr->deposits;

    //se calculeaza starea ideala
    node_state ideal_state, iter_state;
    ideal_state.node_nr = to;
    ideal_state.mask = included_nodes;
    ideal_state.cost = INT_MAX;
    iter_state.mask = 0;

    //se calculeaza starea initiala si se pune in coada
    add_to_vis(&iter_state.mask, from);
    iter_state.node_nr = from;
    iter_state.cost = 0;
    GenQueue* iter_queue, *vis_queue;
    init_gen_q(&iter_queue);
    init_gen_q(&vis_queue);
    add_gen_queue(iter_queue, &iter_state, sizeof(node_state));
    
    while(!is_empty_gen_q(iter_queue)){

        //se obtine primul nod din coada
        node_state* curr_node = (node_state*)iter_queue->head->data;
        uint32_t curr_mask = curr_node->mask;
        int curr_index = curr_node->node_nr;
        double curr_cost = curr_node->cost;

        //se verifica daca e solutie
        if(state_is_solution(&ideal_state, curr_node)){
            if(curr_node->cost < min_cost){
                min_cost = curr_node->cost;
            }
        } else {

            //in caz contrar se parcurg vecinii
            node* iter = gr->neighbours[curr_index];
            while(iter){
                iter_state.mask = curr_mask;
                add_to_vis(&iter_state.mask, iter->node_num);
                iter_state.node_nr = iter->node_num;
                iter_state.cost = curr_cost + get_cost(gr->neighbours[curr_index], iter->node_num);
                    //se adauga in coada daca nu a fost deja vizitat si 
                    //daca e in domeniu
                    if(!in_queue(vis_queue, &iter_state, states_are_equal)&&
                        !state_out_of_bounds(bounds, iter_state.mask)){
                        add_gen_queue(iter_queue, &iter_state, sizeof(node_state));
                    }
                iter=iter->next;
            }
            add_gen_queue(vis_queue, curr_node, sizeof(node_state));
        }
        void* elem = pop_gen_queue(iter_queue);
        if(elem){
            free(elem);
        }
    }
    free(iter_queue);
    free_gen_q(&vis_queue);
    return min_cost;
}

/*
*Algoritmul lui Dijkstra implementat cu un vector
*Returneaza distanta minima dintre from si to
*si calculeaza vectorul de parinti pentru reconstruirea traseului
*/
double Dijkstra_vec(graph* gr, int from, int to, int* parents)
{
    double* dist = malloc(gr->nr_nodes*sizeof(double));
    int* vis = calloc(gr->nr_nodes, sizeof(int));
    for(int i=0;i<gr->nr_nodes;i++){
        //pentru infinit am ales INT_MAX-1000
        dist[i] = INT_MAX-1000;
        parents[i] = -1;
    }
    dist[from] = 0;
    for(int i=0;i<gr->nr_nodes;i++){
        int min = INT_MAX, imin=0;
        //calculul minimului din vector
        for(int j=0;j<gr->nr_nodes;j++){
            if(dist[j]<min&& vis[j]==0){
                min = dist[j];
                imin =j;
            }
        }
        vis[imin]=1;
        node* iter = gr->neighbours[imin];

        //actualizarea distantelor acolo unde este cazul
        while(iter){
            if(dist[imin]+get_cost(gr->neighbours[imin], iter->node_num) <dist[iter->node_num]){
                parents[iter->node_num] = imin;
                dist[iter->node_num]=dist[imin]+get_cost(gr->neighbours[imin], iter->node_num);
            }
            iter=iter->next;
        }
    }
    double rez = dist[to];
    free(dist);
    free(vis);
    return rez;
}

static int cmp_nums(const void* num1, const void* num2)
{
    return (*(uint32_t*)num1 < *(uint32_t*)num2)?1:0;
}

void sort_CTCList(CTCList* list)
{
    qsort(list->components, list->nr_components, sizeof(uint32_t), cmp_nums);
}

void disp_CTCs(CTCList* list)
{
    printf("%d\n", list->nr_components);
    for(int i = 0;i<list->nr_components;i++){
        uint32_t aux = list->components[i];

        //se parcurge bitmask-ul pentru a afisa
        //nodurile din ctc
        for(int j=0;j<32;j++){
            if(is_visited(&aux, j)){

                //se elimina din bitmask pentru a calcula
                //daca este ultimul nod sau nu pentru a nu
                //pune spatii la final
                elim_from_vis(&aux, j);
                if(aux!=0){
                    printf("%d ", j);
                } else {
                    printf("%d", j);
                }
            }
        }
        if(i!=list->nr_components-1){
            puts("");
        }
    }
}

CTCList* Kosaraju(graph* gr, int from)
{
    Stack* st, *st2;
    uint32_t visi1=0, visi2=0;
    CTCList* rez = malloc(sizeof(CTCList));
    rez->nr_components=0;
    rez->components=malloc(gr->nr_nodes*sizeof(uint32_t));
    init_st(&st);
    init_st(&st2);    
    //primul DFS
    DFS_graph(gr, from, &visi1, st);
    graph* tran = transpose_graph(gr);
    tran->deposits = gr->deposits;
    tran->nr_deposits = gr->nr_deposits;
    //se marcheaza depozitele ca vizitate
    //pentru ca altfel tot graful ar fi o CTC
    visi1=gr->deposits;
    visi2=visi1;
    while(!is_empty_st(st)){
        visi2=visi1;

        //pentru al doilea DFS se foloseste o stiva
        //inutila, nu voiam sa modific functia initiala
        DFS_graph(tran, st->top->data, &visi1, st2);
        if((visi1^visi2)!=0){
            rez->components[rez->nr_components]=visi1^visi2;
            rez->nr_components++;
        }

        //se elimina din stiva nodurile vizitate
        while(!is_empty_st(st) && is_visited(&visi1, st->top->data)){
            pop_stack(st);
        }
    }   
    free_graph(tran);
    free_st(&st);
    free_st(&st2);
    return rez;
}