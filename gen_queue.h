#ifndef GEN_QUEUE_H
#define GEN_QUEUE_H

#include <stdint.h>

typedef struct node_gen_q {
    void* data;
    struct node_gen_q* next;
}node_gen_q;


typedef struct GenQueue {
    node_gen_q* head, * tail;
    int len;
}GenQueue;

void free_node_gen_q(node_gen_q* nd);
void init_gen_q(GenQueue** q);
int in_queue(GenQueue* q, void* elem, int (*equals)(const void*, const void*));
GenQueue* copy_gen_q(GenQueue **q);
void free_gen_q(GenQueue**q);
int add_gen_queue(GenQueue* q, void* data, size_t size);
void* pop_gen_queue(GenQueue* q);
void print_gen_q(GenQueue* q, void (*dispfct)(void*));
int is_empty_gen_q(GenQueue* q);


#endif