// Grupo TA02
// Nome: Antonio Rodrigues Rigolino NUSP: 11795791
// Nome: Gustavo Henrique Brunelli NUSP: 11801053
// Nome: João Guilherme Jarochinski Marinho NUSP: 10698193
// Nome: Matheus Henrique de Cerqueira Pinto NUSP: 11911104


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>
#include <mpi.h>

// Struct para que possamos realizar a redução e manter o menor caminho relacionado
typedef struct Minimum {
    int cost;
    int path[];
} Minimum;

Minimum *local_min, *global_minimum;
int N;
int *adj_matrix;

// Função de redução sobre o caminho mínimo
// Compara custo e sobrescreve o caminho para a saída
void reduce_path(void* in, void* inout, int* length, MPI_Datatype* data_type) {
    (void)data_type;

    if (((Minimum*)in)->cost < ((Minimum*)inout)->cost) {
        memcpy(inout, in, *length);
    }
}

// Função que calcula o custo dado um caminho completo
int calculate_cost(int *path, int size) {
    int cost = 0, idx = 0;
    for (idx = 0; idx < size - 1; idx++)
        cost += adj_matrix[path[idx] + N * path[idx + 1]];
    cost += adj_matrix[path[idx] + N * path[0]];

    return cost;
}

// Função que inverte as posições do caminho
void reverse(int *first, int *last) {
    while (first != last && first != --last) {
        int t = *first;
        *first = *last;
        *last = t;
        ++first;
    }
}

// Função que calcula a próxima permutação do caminho lexicograficamente maior
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

// Função que gera todos os caminhos possíveis e calcula seus custos usando permutação
void tsp(int *path, int start, int end) {
    while (next_permutation(&path[start], &path[end])) {
        int cost = calculate_cost(path, end);

        // Checando se a solução atual é melhor que a encontrada anteriormente
        if (cost < local_min->cost) {
            local_min->cost = cost;
            for (int i = 0; i < N; i++)
                local_min->path[i] = path[i];
        }
    }

    // Teste para a última solução
    int cost = calculate_cost(path, end);
    if (cost < local_min->cost) {
        local_min->cost = cost;
        for (int i = 0; i < N; i++)
            local_min->path[i] = path[i];
    }
}

int main(int argc, char *argv[]) {
    // Incializando o OpenMPI
    int size, rank, root = 0;
   	MPI_Init(&argc, &argv);
   	MPI_Comm_size(MPI_COMM_WORLD, &size);
   	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    
    // Resgatando a dimensão da matriz da linha de comando
    if (argc != 2) {
        if (rank == 0)printf("Uso: %s N\n", argv[0]);
        return 1;
    }
    
    N = atoi(argv[1]);
    if (N <= 1){
        if(rank == 0) printf("Valor de N inválido, retornando...\n");
        return 1;
    }
    
    // Gerando matriz de adjacência no processo 0
    adj_matrix = (int *) malloc(N * N * sizeof(int));
    if (rank == root) {    
        srand(1); // Pseudo random - toda iteração será igual

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                adj_matrix[i * N + j] = (i != j) * (rand() % 9 + 1) ;
            }
        }
    }

    // Fazendo o broadcast da matriz gerada para os outros processos
    MPI_Bcast(adj_matrix, N*N, MPI_INT, root, MPI_COMM_WORLD);

    // Inicializando variáveis de custo e caminho locais
    local_min = malloc((N + 1) * sizeof(int));
    local_min->cost = __INT_MAX__;
    int *path = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        path[i] = i;
        local_min->path[i]= i;
    }

    // Começando a calcular o tempo
    double start_time = 0, end_time = 0;
    if (rank == root) start_time = omp_get_wtime();

    // Executando os subproblemas do caixeiro viajante para cada processo
    if (rank != root && rank < N) {
        // Fixando valores iniciais para executar o sub-caminho do Caixeiro Viajante
        path[1] = rank;
        for (int val = 1, idx = 2; idx < N; val++, idx++) {
            val += idx - 1 == rank;
            path[idx] = val;
        }
        tsp(path, 2 , N);
    }
    free(path);
    free(adj_matrix);

    // Reduzindo custo mínimo de todos os processos
    Minimum *global_min = malloc((N + 1) * sizeof(int));

    // Criando redução customizada
    MPI_Op path_reduction;
    MPI_Op_create(reduce_path, true, &path_reduction);
    MPI_Reduce(local_min, global_min, (N + 1) *  sizeof(int), MPI_BYTE, path_reduction, root, MPI_COMM_WORLD);

    // Imprimindo resultados através do processo root
    if (rank == root) {
        end_time = omp_get_wtime();

        printf("Melhor caminho encontrado");
        for (int i = 0; i < N; i++) {
            printf(" %d -", global_min->path[i]);
        }
        printf(" %d\n", global_min->path[0]);


        printf("Custo: %d\n", global_min->cost);
        printf("Tempo gasto na execução: %.4lf\n", end_time - start_time);
    }

    free(local_min);
    free(global_min);
    MPI_Finalize();
    return 0;
}