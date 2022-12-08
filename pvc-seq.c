
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF __INT_MAX__
#define MIN(a,b) (((a)<(b))?(a):(b))

typedef struct Grafo g_t;

struct Grafo
{
    int numVert;
    int **adj;
};

g_t *newGraph(int n)
{

    g_t *G = (g_t *)malloc(sizeof(g_t));

    G->numVert = n;
    G->adj = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        G->adj[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
            G->adj[i][j] = INF;
    }
    return G;
}

void freeGraph(g_t *G)
{
    if (G == NULL)
        return;

    for (int i = 0; i < G->numVert; i++)
        free(G->adj[i]);

    free(G->adj);
    free(G);

    return;
}

int addEdge(g_t *G, int weight, int src, int dest)
{
    if (G == NULL || src > G->numVert || dest > G->numVert || src < 0 || dest < 0)
        return -1;

    G->adj[src][dest] = weight;
    return 0;
}

int bruteforceTSP(g_t *G)
{
    int min_tour = INF;

    for (int i = 0; i < G->numVert; i++)
    {
        int tour = 0;
        for (int j = 0; j < G->numVert; j++)
        {
            int k = (j + 1) % G->numVert;
            tour += G->adj[j][k];
        }

        if (tour < min_tour)
            min_tour = tour;
    }
}



int* reverse(int* arr,int size, int src, int dest)
{
    int* aux = (int*)malloc(size*sizeof(int));
    int k = 0;
    for (k = 0; k < src; k++)
    {
        aux[k] = arr[k];
    }
    for (int i = dest; i > src; i--)
    {
        aux[k] = arr[i];
        k++;
    }
    return aux;
}

int nextPermutation(int* arr, int size)
{
    int n = size; 
    int i, j;
 
    // Find for the pivot element.
    // A pivot is the first element from
    // end of sequencewhich doesn't follow
    // property of non-increasing suffix
    int flag = 0;
    for (i = n - 2; i >= 0; i--) {
        if (arr[i] < arr[i + 1]) {
            flag = 1;
            break;
        }
    }

    if (flag == 0){
        return 1;
    }
 
    // Check if pivot is not found
    if (i < 0) {
        reverse(arr,size,0,size);
    }
 
    // if pivot is found
    else {
 
        // Find for the successor of pivot in suffix
        for (j = n - 1; j > i; j--) {
            if (arr[j] > arr[i]) {
                break;
            }
        }
 
        // Swap the pivot and successor
        int aux = arr[i];
        arr[i] = arr[j];
        arr[j] = aux;
 
        // Minimise the suffix part
        reverse(arr,size, i + 1, size);
    }
    return 0;
}

int shortest_path_sum(g_t* G)
{
    int source = 0;
    int* nodes = (int*)malloc(G->numVert*sizeof(int));
    int k = 0;
    for (int i = 0; i < G->numVert; i++)
    {
        if (i != source)
        {
            nodes[k] = i;
            k++;
        }
    }
    int shortest_path = INT_MAX;
    while (nextPermutation(nodes,k) == 0)
    {
        int path_weight = 0;

        int j = source;
        for (int i = 0; i < k; i++)
        {
            path_weight += G->adj[j][nodes[i]];
            j = nodes[i];
        }
        path_weight += G->adj[j][source];

        shortest_path = MIN(shortest_path, path_weight);
    }
    return shortest_path;
}

int main(int argc, char *argv[])
{
    g_t *G = newGraph(3);
    addEdge(G, 1, 0, 1);
    addEdge(G, 2, 1, 2);

    int min = shortest_path_sum(G);
    printf("O min ficou = %d\n", min);

    freeGraph(G);
    return 0;
}