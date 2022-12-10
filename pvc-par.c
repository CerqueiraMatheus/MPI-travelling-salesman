#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>
// #include <mpi.h>

int local_min_cost, N;
int *local_best_path, *adj_matrix;

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
    while (next_permutation(&path[start], &path[end - 1])) {
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
    
    int size, rank, tag, provided, root = 0;
   	MPI_Init(&argc, &argv);
   	MPI_Comm_size(MPI_COMM_WORLD, &size);
   	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   	MPI_Status status;

    // Resgatando a dimensão da matriz da linha de comando
    if (argc != 2) {
        printf("Uso: %s N\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);

    // Gerando matriz de adjacência no processo 0
    adj_matrix = (int *) malloc(N * N * sizeof(int));
    if (rank == root) {    
        srand(1); // ! Pseudo random - toda iteração será igual
        printf("Matriz de adjacência (col = atual, linha = próximo):\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                adj_matrix[i * N + j] = (i != j) * (rand() % 9 + 1) ;
                printf("%d ", adj_matrix[i * N + j]);
            }
            printf("\n");
        }
    }

    // Fazendo o broadcast da matriz gerada para os outros processos
    MPI_Bcast(adj_matrix, N*N, MPI_INT, root, MPI_COMM_WORLD);

    // Inicializando variáveis de custo e caminho locais
    local_min_cost = __INT_MAX__;
    local_best_path = malloc(N * sizeof(int));
    int *path = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        path[i] = i;
        local_best_path[i] = i;
    }

    // Começando a calcular o tempo
    double start_time = 0, end_time = 0;
    if (rank == 0) start_time = omp_get_wtime();

    // Executando os subproblemas do caixeiro viajante para cada processo
    if (rank != 0) {
        // Fixando valores iniciais para executar o sub-caminho do Caixeiro Viajante
        path[1] = rank;
        for (int val = 1, idx = 2; idx < N; val++, idx++) {
            val += idx - 1 == rank;
            path[idx] = val;
        }

        tsp(path, 2, N);
    }
    free(path);
    free(adj_matrix);


    // Reduzindo custo mínimo de todos os processos
    int global_min_cost;
    MPI_Reduce(&local_min_cost, &global_min_cost, 1, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);

    // 
    free(local_best_path);

    // Imprimindo resultados através do processo root
    if (rank == root) {
        end_time = omp_get_wtime();

        // Resultado
        // printf("\n\nMelhor caminho encontrado");
        // for (int i = 0; i < N; i++) {
        //     printf(" %d -", global_best_path[i]);
        // }
        // printf(" %d\n", global_best_path[0]);


        printf("Custo: %d\n", global_min_cost);
        printf("Tempo gasto na execução: %.4lf\n", end_time - start_time);


    }
}