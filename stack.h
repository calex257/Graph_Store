#ifndef STACK_H
#define STACK_H

#include "node.h"

typedef struct Stack{
    struct node_sq* top;
    int len;
} Stack;


void init_st(Stack** s);
int add_stack(Stack* s, int node_num);
void free_st(Stack** s);
int pop_stack(Stack*s);
void print_st(Stack *s);
int is_empty_st(Stack*s);


#endif