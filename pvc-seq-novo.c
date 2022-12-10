#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define NO_INICIO 0
#define INF INT_MAX
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// Definimos uma struct para representar um grafo
typedef struct Grafo grafo_t;

struct Grafo {
    int num_vertices;
    int **adj;
};

typedef struct {
    int custo;
    int *caminho;
} Resultado;

grafo_t *novo_grafo(int n) {
    // Alocamos memória para a struct grafo
    grafo_t *grafo = (grafo_t *)malloc(sizeof(grafo_t));

    // Inicializamos o número de vértices do grafo
    grafo->num_vertices = n + 1;

    // Alocamos memória para a matriz de adjacência do grafo
    grafo->adj = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        grafo->adj[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) grafo->adj[i][j] = (i == j) ? 0 : INF;
    }

    // Retornamos o grafo criado
    return grafo;
}

void libera_grafo(grafo_t *grafo) {
    // Se o grafo for nulo, não há nada a ser feito
    if (grafo == NULL) return;

    // Liberamos a memória alocada para a matriz de adjacência
    for (int i = 0; i < grafo->num_vertices; i++) free(grafo->adj[i]);
    free(grafo->adj);

    // Liberamos a memória alocada para a struct grafo
    free(grafo);

    return;
}

int adiciona_aresta(grafo_t *grafo, int peso, int src, int dest) {
    // Se o grafo for nulo ou os índices de origem ou destino forem inválidos,
    // retornamos -1
    if (grafo == NULL || src > grafo->num_vertices ||
        dest > grafo->num_vertices || src < 0 || dest < 0)
        return -1;

    // Adicionamos a aresta ao grafo
    grafo->adj[src][dest] = peso;

    // Retornamos 0 para indicar que a operação foi bem sucedida
    return 0;
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

Resultado *tsp(grafo_t *grafo) {
    Resultado *resultado = (Resultado *)malloc(sizeof(Resultado));
    resultado->caminho = calloc(grafo->num_vertices + 1, sizeof(int));
    resultado->custo = INT_MAX;

    // Inicializar o vetor de permutações com a sequência de 1 a n-2
    int *perm = malloc(grafo->num_vertices * sizeof(int));
    for (int i = 0; i < grafo->num_vertices - 1; i++) perm[i] = i + 1;
    for (int i = 0; i < grafo->num_vertices - 1; i++) {
        printf("%d-", perm[i]);
    }
    printf("\n");

    do {
        int custo_atual = 0;

        // Adicionar custo do primeiro ao segundo
        custo_atual += grafo->adj[NO_INICIO][perm[0]];

        // Calcular o custo da rota atual
        for (int i = 0; i < grafo->num_vertices - 2; i++) {
            custo_atual += grafo->adj[perm[i]][perm[i + 1]];
        }

        // Adicionar o custo do último ao primeiro vértice
        custo_atual += grafo->adj[perm[grafo->num_vertices - 2]][NO_INICIO];

        printf("custos -> novo %d e antigo %d\n", custo_atual,
               resultado->custo);
        // Atualizar o resultado se necessário
        if (custo_atual < resultado->custo) {
            resultado->custo = custo_atual;
            for (int i = 1; i < grafo->num_vertices; i++)
                resultado->caminho[i] = perm[i - 1];
        }

        printf("permutação atual: \n");
        for (int i = 0; i < grafo->num_vertices - 1; i++) {
            printf("%d ", perm[i]);
        }
        printf("\n");
    } while (next_permutation(perm, perm + grafo->num_vertices - 1));

    free(perm);

    return resultado;
}

void gera_grafo_aleatorio(grafo_t *grafo) {
    // Inicializamos a seed do gerador de números aleatórios como 42
    srand(42);

    // Para cada vértice do grafo
    for (int i = 0; i < grafo->num_vertices; i++)
        for (int j = 0; j < grafo->num_vertices; j++)
            if (i != j) adiciona_aresta(grafo, rand() % 100 + 1, i, j);
}

void exibe_grafo(grafo_t *grafo) {
    // Verificamos se o grafo é nulo
    if (grafo == NULL) return;
    // Percorremos a matriz de adjacência do grafo
    for (int i = 0; i < grafo->num_vertices; i++) {
        for (int j = 0; j < grafo->num_vertices; j++) {
            // Exibimos o peso da aresta entre os vértices i e j
            printf("%d ", grafo->adj[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    // Verificamos se o número de argumentos passados é válido
    if (argc != 2) {
        printf("Uso: %s N\n", argv[0]);
        return 1;
    }

    // Convertemos o argumento N para inteiro
    int n = atoi(argv[1]);

    // Verificamos se o valor de N é válido
    if (n <= 1) {
        printf("O valor de N deve ser maior que 1\n");
        return 1;
    }

    // Criamos um novo grafo com n vértices
    grafo_t *grafo = novo_grafo(n);

    Resultado *resultado;

    // Geramos o grafo de maneira aleatória
    gera_grafo_aleatorio(grafo);

    exibe_grafo(grafo);

    // Calculamos a menor rota usando o algoritmo de força bruta
    resultado = tsp(grafo);

    // Exibimos o resultado
    printf("Menor rota: %d\n", resultado->custo);
    for (int i = 0; i < grafo->num_vertices + 1; i++) {
        printf("%d", resultado->caminho[i]);
    }
    printf("\n");

    // Liberamos a memória alocada para o grafo
    libera_grafo(grafo);

    return 0;
}