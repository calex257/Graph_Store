#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "adj_list.h"

int length_al(node* list)
{
    int cnt = 0;
    node* p = list;
    while (p) {
        p = p->next;
        cnt++;
    }
    return cnt;
}

void print_list_al(node* list, void (*disp_func)(int))
{
    node* aux = list;
    while(aux!=NULL){
        disp_func(aux->node_num);
        aux = aux->next;
    }
}

static node* new_node(int node_num, double cost)
{
    node* aux = (node*)malloc(sizeof(*aux));
    aux->node_num = node_num;
    aux->cost = cost;
    aux->next = NULL;
    return aux;
}

void push_al(node** list, int node_num, double cost)
{
    node* new = new_node(node_num, cost);
    new->next = *list;
    *list = new;
}

void free_list_al(node** list)
{
    node* ls = *list, *aux;
    while(ls!=NULL){
        aux = ls->next;
        free(ls);
        ls = aux;
    }
}
double get_cost(node* list, int to)
{
    node* iter = list;
    while(iter!=NULL){
        if(iter->node_num == to){
            return iter->cost;
        }
        iter=iter->next;
    }
    return INT_MAX;
}


node* remove_node(node** list, int node_nr)
{
    if ((*list)==NULL) {
        fprintf(stderr, "empty list, cannot remove\n");
        return NULL;
    }
    node* p, * q;
    p = q = *list;
    if ((*list)->node_num == node_nr) {
        (*list) = (*list)->next;
        return p;
    }
    for (;(p&&p->node_num!=node_nr);q = p, p = p->next);
    if(p==NULL){
        fprintf(stderr, "node not found\n");
        return NULL;
    }
    q->next = p->next;
    return p;
}