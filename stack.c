#include <stdlib.h>
#include <stdio.h>
#include "stack.h"


void init_st(Stack** s)
{
    *s = (Stack*)malloc(sizeof(**s));
    (*s)->top = NULL;
    (*s)->len = 0;
}

void free_st(Stack** s)
{
    node_sq* iter = (*s)->top, * aux;
    while (iter!=NULL) {
        if(iter){
            aux = iter->next;
            free_node(iter);
            iter = aux;
        }
    }
    free(*s);
}

int add_stack(Stack* s, int node_num)
{
    node_sq* new = (node_sq*)malloc(sizeof(*new));
    if(new==NULL){
        return 0;
    }
    new->data = node_num;
    if (is_empty_st(s)) {
        new->next = NULL;
        s->top = new;
    }
    else {
        new->next = s->top;
        s->top = new;
    }
    s->len++;
    return 1;
}

int pop_stack(Stack*s)
{
    if(is_empty_st(s)) {
        return -1;
    }
    node_sq* nd = s->top;
    int aux = nd->data;
    nd = s->top;
    s->top=s->top->next;
    s->len--;
    free(nd);
    return aux;
}

void print_st(Stack *s)
{
    node_sq* iter = s->top;
    while(iter){
        printf("%3d ", iter->data);
        iter=iter->next;
    }
    puts("");
}

int is_empty_st(Stack*s)
{
    return s->top==NULL;
}