#ifndef NODE_H
#define NODE_H


typedef struct node_sq {
    int data;
    struct node_sq* next;
}node_sq;

void free_node(node_sq* nd);

#endif