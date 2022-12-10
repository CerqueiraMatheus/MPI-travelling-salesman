CFLAGS += -Wall -Wextra -pedantic -O3 -fopenmp
NPROCS ?= 4
NUMS ?= 4


all: pcv-seq pcv-par

seq: pcv-seq
	@./pcv-seq $(NUMS)

par: pcv-par
	@mpirun -np $(NPROCS) pcv-par $(NUMS)

par-cluster: pcv-par
	@mpirun -np $(NPROCS) --hostfile hostfile.txt pcv-par $(NUMS)

pcv-seq: pcv-seq.c
	@$(CC) pcv-seq.c -o pcv-seq $(CFLAGS)

pcv-par: pcv-par.c
	@mpicc pcv-par.c -o pcv-par $(CFLAGS)

clean:
	@rm -f pcv-seq pcv-par


.PHONY: seq par clean