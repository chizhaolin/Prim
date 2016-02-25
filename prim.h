#ifndef _prim_h
#define _prim_h

#define MAX_VERTEX  9
#define INFINITE    100000

typedef struct node {
    int vertex;
    int weight;
    struct node* next;
} t_node, t_queue;

typedef struct mst {
    int u;
    int v;
    int weight;
    struct mst* next;
}t_mst;

#endif
