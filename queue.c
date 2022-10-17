#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void init_q(Queue** q)
{
    (*q) = (Queue*)malloc(sizeof(**q));
    if (*q == NULL) {
        exit(1);
    }
    (*q)->head = (*q)->tail = NULL;
    (*q)->len = 0;
}

void free_q(Queue** q)
{
    node_sq* iter = (*q)->head, * aux;
    while (iter) {
        aux = iter->next;
        free(iter);
        iter = aux;
    }
    free(*q);
}

int add_queue(Queue* q, int node_num)
{
    node_sq* new = (node_sq*)malloc(sizeof(*new));
    if (new == NULL) {
        return 0;
    }
    new->data = node_num;
    new->next = NULL;
    if (q->head == NULL) {
        q->head = q->tail = new;
    }
    else {
        q->tail->next = new;
        q->tail = new;
    }
    q->len++;
    return 1;
}

int pop_queue(Queue* q)
{
    if (q->head == NULL) {
        return -1;
    }
    node_sq* nd = q->head;
    int aux = q->head->data;
    q->head=q->head->next;
    q->len--;
    free(nd);
    return aux;
}

void print_q(Queue* q)
{
    node_sq* iter = q->head;
    while(iter) {
        printf("%3d ", iter->data);
        iter=iter->next;
    }
    puts("");
}

int is_empty_q(Queue* q)
{
    return q->len==0;
    return q->head==NULL;
}

void free_node(node_sq* nd)
{
    if(nd == NULL){
        return;
    }
    free(nd);
}