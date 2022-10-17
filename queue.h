#ifndef QUEUE_H
#define QUEUE_H
#include "node.h"

typedef struct Queue {
    struct node_sq* head, * tail;
    int len;
}Queue;

void init_q(Queue**q);
void free_q(Queue**q);
int add_queue(Queue* q, int node_num);
int pop_queue(Queue* q);
void print_q(Queue* q);
int is_empty_q(Queue* q);
void free_node(node_sq* nd);

#endif