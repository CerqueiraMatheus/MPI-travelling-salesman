#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>

int min_cost, N;
int *adj_matrix, *best_path;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int calculate_cost(int *path, int size) {
    int cost = 0, idx = 0;
    for (idx = 0; idx < size - 1; idx++)
        cost += adj_matrix[path[idx] + N * path[idx + 1]];
    cost += adj_matrix[path[idx] + N * path[0]];

    return cost;
}

// Função recursiva que gera todos os caminhos possíveis e calcula seus custos usando permutação
void tsp(int *path, int start, int end) {
    // Caso base: nova permutação encontrada, calculamos então o custo
    if (start == end) { 
        int cost = calculate_cost(path, end);

        // Checando se a solução atual é melhor que a encontrada anteriormente
        if (cost < min_cost) {
            min_cost = cost;
            for (int i = 0; i < N; i++)
                best_path[i] = path[i];
        }
    }
    else { // Realizanto trocas para gerar novas permutações
        for (int i = start; i < end; i++) {
            swap(&path[start], &path[i]);
            tsp(path, start + 1, end);
            swap(&path[start], &path[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    // Initialize the MPI environment
    int num_threads, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_threads);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Resgatando a dimensão da matriz da linha de comando
    if (argc != 2) {
        printf("Uso: %s N\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);

    
    // Gerando matriz de adjacência
    srand(1); // ! Pseudo random - toda iteração será igual
    printf("Matriz de adjacência (col = atual, linha = próximo):\n");
    adj_matrix = (int *) malloc(N * N * sizeof(int));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            adj_matrix[i * N + j] = (i != j) * (rand() % 9 + 1) ;
            printf("%d ", adj_matrix[i * N + j]);
        }
        printf("\n");
    }

    // Inicializando variáveis de custo e caminho
    min_cost = __INT_MAX__;
    best_path = malloc(N * sizeof(int));
    int *path = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        path[i] = i;
        best_path[i] = i;
    }

    // Medindo tempo de execução do Caixeiro Viajante Sequencial
    double start = omp_get_wtime();
    tsp(path, 0, N);
    double end = omp_get_wtime();

    // Resultado
    printf("\n\nMelhor caminho encontrado");
    for (int i = 0; i < N; i++) {
        printf(" %d -", best_path[i]);
    }
    printf(" %d\n", best_path[0]);
    printf("Custo: %d\n", min_cost);
    printf("Tempo gasto na execução: %.4lf\n", end - start);

    free(best_path);
    free(path);
}