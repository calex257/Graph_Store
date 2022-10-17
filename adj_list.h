#ifndef ADJ_LIST_H
#define ADJ_LIST_H

typedef struct node {
    int node_num;
    double cost;
    struct node* next;
} node;

int length_al(node* list);
void print_list_al(node* list, void (*disp_func)(int));
void free_list_al(node** list);
void push_al(node** list, int node_num, double cost);
node* remove_node(node** list, int n);
double get_cost(node* list, int to);

#endif