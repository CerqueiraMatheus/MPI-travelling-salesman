#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HIGH 10000
#define N 4

int main(void) {
    seed(1);

    int *adj_matrix = malloc(N*N*sizeof(int));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            adj_matrix[i * N + j] = (i == j * HIGH) + (rand() % 10 + 1);

    int *path = calloc(N, sizeof(int));
    int *visited = calloc(N, sizeof(int));
    memset(path, -1, N * sizeof(int));

    path[0] = 0;
    int top = -1;
    int min_cost = N * HIGH;
    int curr_cost = 0;

    // Usando blacktrappers
    for (int i = 0; i < N; i++) {
        
    }

    
   
        
    free(path);
}