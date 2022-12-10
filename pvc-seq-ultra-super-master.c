#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>

#define HIGH 10000

int min_cost, N;
int *adj_matrix, *best_path;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Função recursiva que gera todos os caminhos possíveis e calcula seus custos usando permutação
void tsp(int *path, int start, int end) {
    if (start == end) { 
        // Caso base: nova permutação, calculamos então o custo
        int cost = 0, idx = 0;
        for (idx = 0; idx < end - 1; idx++)
            cost += adj_matrix[path[idx] + N * path[idx + 1]];
        cost += adj_matrix[path[idx] + N * path[0]];

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
    // Resgatando a dimensão da matriz da linha de comando
    if (argc != 2) {
        printf("Uso: %s N\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);

    srand(1); // ! Pseudo random - toda iteração será igual
    
    // Gerando matriz de adjacência
    adj_matrix = (int *) malloc(N * N * sizeof(int));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            adj_matrix[i * N + j] = (i == j * HIGH) + (rand() % 10 + 1);
        }
    }

    // Inicializando variáveis de custo e caminho
    min_cost = HIGH;
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
    printf("Melhor caminho encontrado");
    for (int i = 0; i < N; i++) {
        printf(" %d -", best_path[i]);
    }
    printf(" %d\n", best_path[0]);

    printf("Custo: %d\n", min_cost);
    printf("Tempo gasto na execução: %.2lf\n", end - start);
}