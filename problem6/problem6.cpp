#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>

static int partitionSize(int M, int P, int r) {
    int L = M / P;
    int R = M % P;
    return L + (r < R ? 1 : 0);
}

static void scatterDist(int I, int P, int &p, int &i) {
    p = I % P; //which process owns the element I
    i = I / P; //the local index inside the process
}

static int linearStartIndex(int M, int P, int row) {
    int start = 0; 

    for (int r = 0; r < row; r++) {
        start += partitionSize(M, P, r);
    }
    return start;
}




int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank;
    int nprocs; 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);





    MPI_Finalize();
    return 0;
}