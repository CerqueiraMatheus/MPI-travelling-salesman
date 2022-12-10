# MPI-travelling-salesman

## Grupo TA02
- Nome: Antonio Rodrigues Rigolino NUSP: 11795791
- Nome: Gustavo Henrique Brunelli NUSP: 11801053
- Nome: João Guilherme Jarochinski Marinho NUSP: 10698193
- Nome: Matheus Henrique de Cerqueira Pinto NUSP: 11911104

## Execução
#### Sequencial
- Compilação: `make pcv-seq`
- Execução: `make seq` para rodar com dados pré-definidos ou `./pcv-seq DIMENSAO`

#### Paralelo
- Compilação: `make pcv-par`
- Execução em uma máquina: `make par` ou `mpirun [--oversubscribe] -np NPROCS pcv-par DIMENSAO`
- Execução no cluster: Cadastrar os hosts de interesse em hostfile.txt e então executar `make par-cluster` ou `mpirun [--oversubscribe] -np NPROCS --hostfile hostfile.txt pcv-par DIMENSAO`

