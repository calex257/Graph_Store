#include <stdlib.h>
#include "node.h"

void free_node(node_sq* nd)
{
    if(nd == NULL){
        return;
    }
    free(nd);
}