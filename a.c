#include <stdio.h> 

int main(void) {

    int path[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int N = 10;
    for (int rank = 1; rank < N; rank++) {
        path[1] = rank;

        for (int val = 1, idx = 2; idx < N; val++, idx++) {
            val += idx - 1 == rank;
            path[idx] = val;
        }

        printf("\n\n");
        for (int i = 0; i < N; i++)
            printf("%d - ", path[i]);

        path[0] = 0;
        path[1] = 1;
        path[2] = 2;
        path[3] = 3;
        path[4] = 4;
        path[5] = 5;
        path[6] = 6;
        path[7] = 7;
        path[8] = 8;
        path[9] = 9;
    }
}