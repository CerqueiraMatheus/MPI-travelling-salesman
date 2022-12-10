# Grupo TA02
# Nome: Antonio Rodrigues Rigolino NUSP: 11795791
# Nome: Gustavo Henrique Brunelli NUSP: 11801053
# Nome: João Guilherme Jarochinski Marinho NUSP: 10698193
# Nome: Matheus Henrique de Cerqueira Pinto NUSP: 11911104


CFLAGS += -Wall -Wextra -pedantic -O3 -fopenmp
NPROCS ?= 20
NUMS ?= 11


all: pcv-seq pcv-par

seq: pcv-seq
	@./pcv-seq $(NUMS)

par: pcv-par
	@mpirun -np $(NPROCS) pcv-par $(NUMS)

par-cluster: pcv-par
	@mpirun -np $(NPROCS) --oversubscribe --hostfile hostfile.txt pcv-par $(NUMS)

pcv-seq: pcv-seq.c
	@$(CC) pcv-seq.c -o pcv-seq $(CFLAGS)

pcv-par: pcv-par.c
	@mpicc pcv-par.c -o pcv-par $(CFLAGS)

clean:
	@rm -f pcv-seq pcv-par


.PHONY: seq par clean