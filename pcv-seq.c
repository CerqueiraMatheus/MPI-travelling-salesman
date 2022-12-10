#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>

int local_min_cost, N;
int *adj_matrix, *local_best_path;

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

void reverse(int *first, int *last) {
    while (first != last && first != --last) {
        int t = *first;
        *first = *last;
        *last = t;
        ++first;
    }
}

int next_permutation(int *first, int *last) {
    if (first == last) return 0;

    int *i = last;
    if (first == --i) return 0;

    while (1) {
        int *ii = i;
        if (*--i < *ii) {
            int *j = last;
            while (!(*i < *--j))
                ;
            int t = *i;
            *i = *j;
            *j = t;
            reverse(ii, last);
            return 1;
        }
        if (i == first) {
            reverse(first, last);
            return 0;
        }
    }
}

// Função recursiva que gera todos os caminhos possíveis e calcula seus custos usando permutação
void tsp(int *path, int start, int end) {
    while (next_permutation(&path[start], &path[end])) {
        int cost = calculate_cost(path, end);

        // Checando se a solução atual é melhor que a encontrada anteriormente
        if (cost < local_min_cost) {
            local_min_cost = cost;
            for (int i = 0; i < N; i++)
                local_best_path[i] = path[i];
        }
    }
    int cost = calculate_cost(path, end);

    // Checando se a solução atual é melhor que a encontrada anteriormente
    if (cost < local_min_cost) {
        local_min_cost = cost;
        for (int i = 0; i < N; i++)
            local_best_path[i] = path[i];
    }
}

int main(int argc, char *argv[]) {
    // Resgatando a dimensão da matriz da linha de comando
    if (argc != 2) {
        printf("Uso: %s N\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);

    
    // Gerando matriz de adjacência
    srand(1); // Pseudo random - toda iteração será igual
    adj_matrix = (int *) malloc(N * N * sizeof(int));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            adj_matrix[i * N + j] = (i != j) * (rand() % 9 + 1) ;
        }
    }

    // Inicializando variáveis de custo e caminho
    local_min_cost = __INT_MAX__;
    local_best_path = malloc(N * sizeof(int));
    int *path = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        path[i] = i;
        local_best_path[i] = i;
    }

    // Medindo tempo de execução do Caixeiro Viajante Sequencial
    double start = omp_get_wtime();
    tsp(path, 1, N);
    double end = omp_get_wtime();

    // Resultado
    printf("\n\nMelhor caminho encontrado");
    for (int i = 0; i < N; i++) {
        printf(" %d -", local_best_path[i]);
    }
    printf(" %d\n", local_best_path[0]);
    printf("Custo: %d\n", local_min_cost);
    printf("Tempo gasto na execução: %.4lf\n", end - start);

    free(adj_matrix);
    free(local_best_path);
    free(path);
}