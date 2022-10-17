#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "gen_queue.h"
#include "graph.h"
#include "algos.h"

void tsp_task(graph* gr)
{
    char buffer[50], *p;
    int nr_comps;
    uint32_t included_nodes;
    fgets(buffer, 50, stdin);
    sscanf(buffer, "%d", &nr_comps);
    for(int i=0;i<nr_comps;i++){
        included_nodes = 0;
        fgets(buffer, 50, stdin);
        p=strtok(buffer, " \n\t");
        p=strtok(NULL, " \n\t");
        while(p!=NULL){
            int aux;
            sscanf(p, "%d", &aux);
            add_to_vis(&included_nodes, aux);
            p=strtok(NULL, " \n\t");
        }
        double min = INT_MAX, res;
        //incerc sa incep traseul de la fiecare depozit
        for(int j=0;j<31;j++){
            if(is_visited(&gr->deposits, j)){
                add_to_vis(&included_nodes, j);
                res = TSP_modif(gr, j, j, included_nodes);

                //calculez minimul dintre toate punctele posibile de plecare
                if(res<min){
                    min = res;
                }
                elim_from_vis(&included_nodes, j);
            }
        }
        printf("%.1lf", min);
        if(i!=nr_comps-1){
            puts("");
        }
    }
}

void back_track_road(int* parents,int iter, int end)
{
    if(parents[iter] != end){
        back_track_road(parents, parents[iter], end);
        printf("%d ", parents[iter]);
    }
}

double dijkstra_task(graph* gr)
{
    char buffer[50], * p;
    int source, nr_shops, destination;
    int * parents_leave, *parents_return;
    double total_sum = 0;
    parents_leave = malloc(gr->nr_nodes * sizeof(int));
    parents_return = malloc(gr->nr_nodes * sizeof(int));
    fgets(buffer, 50, stdin);
    sscanf(buffer, "%d", &source);
    fgets(buffer, 50, stdin);
    sscanf(buffer, "%d", &nr_shops);
    fgets(buffer, 50, stdin);
    p = strtok(buffer, " \n\t");
    for (int i = 0;i < nr_shops;i++) {
        sscanf(p, "%d", &destination);
        printf("%d\n", destination);
        double cost_leave = Dijkstra_vec(gr, source, destination, parents_leave);
        printf("%.1lf ", cost_leave);
        double cost_return = Dijkstra_vec(gr, destination, source, parents_return);
        printf("%.1lf\n", cost_return);
        printf("%d ", source);
        back_track_road(parents_leave, destination, source);
        printf("%d ", destination);
        back_track_road(parents_return, source, destination);
        printf("%d\n", source);
        total_sum += cost_leave + cost_return;
        p = strtok(NULL, " \n\t");
    }

    free(parents_leave);
    free(parents_return);
    return total_sum;
}

void ctc_task(graph* gr)
{
    CTCList* ctcs = Kosaraju(gr, 0);
    sort_CTCList(ctcs);
    disp_CTCs(ctcs);
    free(ctcs->components);
    free(ctcs);
}

int main(int argc, char const* argv[])
{
    graph* gr = malloc(sizeof(graph));
    int nr_instructions;
    char buffer[50];
    read_graph(gr, stdin);
    fgets(buffer, 50, stdin);
    sscanf(buffer, "%d", &nr_instructions);
    for (int i = 0;i < nr_instructions;i++) {
        fgets(buffer, 50, stdin);
        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = 0;
        }
        if (strcmp(buffer, "e1") == 0) {
            double result = dijkstra_task(gr);
            printf("%.1lf", result);
            if(i!=nr_instructions-1){
                puts("");
            }
        }
        else if (strcmp(buffer, "e2") == 0) {
            ctc_task(gr);
        }
        else if (strcmp(buffer, "e3") == 0) {
            tsp_task(gr);
        }
        else {
            fprintf(stderr, "invalid op: %s\n", buffer);
        }
    }
    free_graph(gr);
    return 0;
}
