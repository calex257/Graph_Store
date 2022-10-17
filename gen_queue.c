#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "gen_queue.h"

int in_queue(GenQueue* q, void* elem, int (*equals)(const void*, const void*))
{
    node_gen_q* iter = q->head;
    while (iter) {
        if(equals(elem, iter->data)){
            return 1;
        }
        iter = iter->next;
    }
    return 0;
}

void init_gen_q(GenQueue** q)
{
    (*q) = (GenQueue*)malloc(sizeof(**q));
    if (*q == NULL) {
        exit(1);
    }
    (*q)->head = (*q)->tail = NULL;
    (*q)->len = 0;
}

void free_gen_q(GenQueue** q)
{
    node_gen_q* iter = (*q)->head, * aux;
    while (iter) {
        aux = iter->next;
        free_node_gen_q(iter);
        iter = aux;
    }
    free(*q);
}

int add_gen_queue(GenQueue* q, void* data, size_t size)
{
    node_gen_q* new = (node_gen_q*)malloc(sizeof(*new));
    if (new == NULL) {
        return 0;
    }
    new->data = malloc(size*sizeof(char));
    memcpy(new->data, data, size);
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

void* pop_gen_queue(GenQueue* q)
{
    if (q->head == NULL) {
        return NULL;
    }
    node_gen_q* nd = q->head;
    void* aux = q->head->data;
    q->head=q->head->next;
    q->len--;
    free(nd);
    return aux;
}

void print_gen_q(GenQueue* q, void (*dispfct)(void*))
{
    node_gen_q* iter = q->head;
    while(iter) {
        dispfct(iter->data);
        iter=iter->next;
    }
    puts("");
}

int is_empty_gen_q(GenQueue* q)
{
    return q->len==0;
    return q->head==NULL;
}

void free_node_gen_q(node_gen_q* nd)
{
    if(nd == NULL){
        return;
    }
    free(nd->data);
    free(nd);
}