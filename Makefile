CFLAGS += -Wall -Wextra -pedantic -O3 -fopenmp
NPROCS ?= 8
NUMS ?= 5


all: pvc-seq pvc-par

seq: pvc-seq
	@./pvc-seq $(NUMS)

par: pvc-par
	@mpirun -np $(NPROCS) --hostfile hostfile.txt pvc-par $(NUMS)

pvc-seq: pvc-seq.c
	@$(CC) pvc-seq.c -o pvc-seq $(CFLAGS)

pvc-par: pvc-par.c
	@mpicc pvc-par.c -o pvc-par $(CFLAGS)

clean:
	@rm -f pvc-seq pvc-par


.PHONY: seq par clean