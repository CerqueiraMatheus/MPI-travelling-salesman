#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

// estrutura para representar um caminho
typedef struct
{
    int *cities; // vetor de cidades do caminho
    int size;    // tamanho do caminho (número de cidades)
    int cost;    // custo do caminho
} path;

// estrutura para representar o grafo
typedef struct
{
    int n;     // número de cidades no grafo
    int **adj; // matriz de adjacências do grafo
} graph;

// função para criar um novo caminho
path *new_path(int size)
{
    path *p = malloc(sizeof(path));
    p->cities = malloc(size * sizeof(int));
    p->size = size;
    p->cost = INT_MAX;
    return p;
}

// função para criar um novo grafo
graph *new_graph(int n)
{
    graph g = malloc(sizeof(graph));
    g->n = n;
    g->adj = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        g->adj[i] = malloc(n * sizeof(int));
    return g;
}

// função para gerar valores pseudo-aleatórios para os custos
void randomize_costs(graph *g)
{
    // semente para gerar números aleatórios
    srand(time(NULL));

    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
        {
            // gera um número aleatório entre 1 e 10 para o custo da aresta
            int cost = rand() % 10 + 1;
            // armazena o custo na matriz de adjacências
            g->adj[i][j] = cost;
        }
    }
}

// função para calcular o custo de um caminho
int calculate_path_cost(graph *g, path *p)
{
    int cost = 0;

    // percorre todas as cidades do caminho
    for (int i = 0; i < p->size; i++)
    {
        // se a cidade atual não é a última do caminho
        if (i < p->size - 1)
        {
            // adiciona o custo da aresta entre a cidade atual e a próxima
            cost += g->adj[p->cities[i]][p->cities[i + 1]];
        }
        else
        {
            // se a cidade atual é a última, adiciona o custo da aresta
            // entre a última e a primeira cidade do caminho (formando o ciclo)
            cost += g->adj[p->cities[i]][p->cities[0]];
        }
    }

    // armazena o custo calculado no caminho
    p->cost = cost;
    return cost;
}

// função para gerar novos caminhos a partir de um caminho dado
path **generate_new_paths(graph *g, path *p)
{
    // aloca um vetor de caminhos
    path *new_paths = malloc((g->n - p->size) * sizeof(path));

    // para cada cidade que ainda não está no caminho dado
    for (int i = 0; i < g->n; i++)
    {
        bool in_path = false;
        for (int j = 0; j < p->size; j++)
        {
            // verifica se a cidade já está no caminho
            if (p->cities[j] == i)
            {
                in_path = true;
                break;
            }
        }

        // se a cidade não está no caminho, cria um novo caminho
        if (!in_path)
        {
            // cria um novo caminho com o mesmo tamanho do caminho dado mais 1
            path *new_path = new_path(p->size + 1);

            // copia o caminho dado para o novo caminho
            for (int j = 0; j < p->size; j++)
                new_path->cities[j] = p->cities[j];

            // adiciona a nova cidade ao final do caminho
            new_path->cities[p->size] = i;

            // calcula o custo do novo caminho
            calculate_path_cost(g, new_path);

            // adiciona o novo caminho ao vetor de caminhos
            new_paths[p->size] = new_path;
        }
    }

    // retorna o vetor de novos caminhos gerados
    return new_paths;
}

// função para encontrar o caminho mínimo no conjunto de caminhos
path *find_minimum_path(path **paths, int size)
{
    // inicialmente, considera-se que o caminho mínimo é o primeiro do vetor
    path *min_path = paths[0];

    // percorre o vetor de caminhos
    for (int i = 1; i < size; i++)
    {
        // se o custo do caminho atual é menor que o custo do caminho mínimo
        if (paths[i]->cost < min_path->cost)
            // atualiza o caminho mínimo
            min_path = paths[i];
    }

    // retorna o caminho mínimo encontrado
    return min_path;
}

int main(int argc, char **argv)
{
    // verifica se o número de argumentos passados é válido
    if (argc != 2)
    {
        printf("Uso: %s <número de cidades>\n", argv[0]);
        return 1;
    }

    // lê o número de cidades do argumento de linha de comando
    int n = atoi(argv[1]);

    // cria um novo grafo com n cidades
    graph *g = new_graph(n);

    // gera valores pseudo-aleatórios para os custos das arestas
    randomize_costs(g);

    // cria o caminho inicial com apenas a primeira cidade
    path *p = new_path(1);
    p->cities[0] = 0;

    // calcula o custo do caminho inicial
    calculate_path_cost(g, p);

    // gera novos caminhos a partir do caminho inicial
    path **new_paths = generate_new_paths(g, p);

    // encontra o caminho mínimo no conjunto de caminhos gerados
    path *min_path = find_minimum_path(new_paths, n - 1);

    // imprime a relação das cidades do caminho mínimo
    printf("Caminho mínimo: ");
    for (int i = 0; i < min_path->size; i++)
        printf("%d ", min_path->cities[i]);
    printf("\n");

    // imprime o custo do caminho mínimo
    printf("Custo do caminho mínimo: %d\n", min_path->cost);

    // libera a memória alocada
    for (int i = 0; i < n; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
    for (int i = 0; i < n - 1; i++)
        free(new_paths[i]->cities);
    free(new_paths);
    // retorna 0 para indicar sucesso
    return 0;
}