
#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include "prim.h"

// The global set to store the visited vertex
int g_VisitedSet[MAX_VERTEX];
// The global set to store the remaining vertex
int g_RemainingSet[MAX_VERTEX];
// The adjcent list adjacent list
t_queue* g_AdjList[MAX_VERTEX];

t_mst* g_mst;

// Create the new node with the vertex and weight
t_node* createNode(int vertex, int weight)
{
    t_node* pNode;

    pNode = (t_node*)malloc(sizeof(t_node));

    if (pNode != NULL) {
        pNode->vertex = vertex;
        pNode->weight = weight;
        pNode->next = NULL;
    }

    return pNode;
}

// Add new node to the queue
t_queue* enQueue(t_queue* queue, t_node* node)
{
    t_node* pTmp;

    if (queue == NULL)
        return node;

    pTmp = queue;
    while (pTmp->next != NULL)
        pTmp = pTmp->next;

    pTmp->next = node;

    return queue;
}

// Delete the 1st node of the queue
t_queue* deQueue(t_queue* queue)
{
    if (queue != NULL) {
        t_node* pDel;
        pDel = queue;
        queue = queue->next;
        free(pDel);
        pDel = NULL;
    }

    return queue;
}

// Destroy the whole queue
void destroyQueue(t_queue** queue)
{
    while (*queue) {
        *queue = deQueue(*queue);
    }
}

// Add the edge between the 2 verties
void addEdge(int n1, int n2, int weight)
{
    g_AdjList[n1] = enQueue(g_AdjList[n1], createNode(n2, weight));
    g_AdjList[n2] = enQueue(g_AdjList[n2], createNode(n1, weight));
}

t_mst* addList(t_mst* pMst, int u, int v, int weight)
{
    t_mst* pNew;
    t_mst* pTmp;

    pNew = (t_mst*)malloc(sizeof(t_mst));
    if (pNew) {
        pNew->u = u;
        pNew->v = v;
        pNew->weight = weight;
        pNew->next = NULL;
    }

    if (pMst == NULL)
        return pNew;

    pTmp = pMst;
    while (pTmp->next)
        pTmp = pTmp->next;
    pTmp->next = pNew;

    return pMst;
}

// Initialization procedure
void init()
{
    int i;

    for (i = 0; i < MAX_VERTEX; i++) {
        g_VisitedSet[i] = -1;
        g_RemainingSet[i] = i;
        g_AdjList[i] = createNode(i, INFINITE);
    }

    g_mst = NULL;
}

void finalize()
{
    int i;
    
    for (i = 0; i < MAX_VERTEX; i++) {
        g_VisitedSet[i] = -1;
        g_RemainingSet[i] = i;
        destroyQueue(&g_AdjList[i]);
    }

    while (g_mst) {
        t_mst* pDel;
        pDel = g_mst;
        g_mst = g_mst->next;
        free(pDel);
    }
}

// Function to check if the remaining set is empty
int isRemainingSetEmpty()
{
    int i;

    for (i = 0; i < MAX_VERTEX; i++) {
        if (g_RemainingSet[i] != -1)
            return 0;
    }

    return 1;
}

int searchPath(int u, int v)
{
    t_queue* queue;

    queue = g_AdjList[u];
    while (queue) {
        if (queue->vertex == v)
            return queue->weight;
        queue = queue->next;
    }

    return INFINITE;
}

void searchMst()
{
    int i, j;
    int u, v, minU, minV;
    int min = INFINITE;
    int pathUV;

    for (i = 0; i < MAX_VERTEX; i++) {
        if (g_VisitedSet[i] == -1)
            continue;

        v = g_VisitedSet[i];
        for (j = 0; j < MAX_VERTEX; j++) {
            if (g_RemainingSet[j] == -1)
                continue;

            u = g_RemainingSet[j];
            pathUV = searchPath(u, v);
            if (min > pathUV) {
                min = pathUV;
                minU = u;
                minV = v;
            }
        }
    }

    if (min != INFINITE) {
        g_mst = addList(g_mst, minV, minU, min);
        g_VisitedSet[minU] = minU;
        g_RemainingSet[minU] = -1;
    }
}

void prim(int v)
{
    g_VisitedSet[v] = v;
    g_RemainingSet[v] = -1;

    while (!isRemainingSetEmpty()) {
        searchMst();
    }
}


int main(int argc, char* argv[])
{
    int i;
    t_mst* pMst;
    char *cl;

    init();
    addEdge(0, 1, 4);
    addEdge(0, 7, 8);
    addEdge(1, 2, 8);
    addEdge(1, 7, 11);
    addEdge(2, 3, 7);
    addEdge(2, 5, 4);
    addEdge(2, 8, 2);
    addEdge(3, 4, 9);
    addEdge(3, 5, 14);
    addEdge(4, 5, 10);
    addEdge(5, 6, 2);
    addEdge(6, 7, 1);
    addEdge(6, 8, 6);
    addEdge(7, 8, 7);

    printf("argc = %d\n", argc);
    while (argc) {
        printf("argv[%d] = %s\n", argc - 1, argv[argc - 1]);
        argc--;
    }

    cl = GetCommandLineA();
    printf("%s\n", cl);

    // Output the adjacent list
    printf("The adjacent list is: \n");
    for (i = 0; i < MAX_VERTEX; i++) {
        t_queue* p;
        p = g_AdjList[i];
        while (p) {
            printf ("%d ", p->vertex);
            p = p->next;
        }
        printf("\n");
    }

    prim(0);
    printf("Prim's Minimum Spanning Tree (MST) from vertex 0 is: \n");
    pMst = g_mst;
    while (pMst) {
        printf("%d--%d   %d\n", pMst->u, pMst->v, pMst->weight);
        pMst = pMst->next;
    }

    finalize();

    return 0;
}