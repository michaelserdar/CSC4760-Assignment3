#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>

//function to decide on how many elements each rank r gets (same as problem0)
static int partitionSize(int M, int P, int r) {
    int L = M / P;
    int R = M % P;

    return L + (r < R ? 1 : 0);
}


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank;
    int nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    //check for the correct number of arguments 
    if (argc != 3) {
        if (rank == 0) {
            std::cerr << "Usage: ./problem2 <M> <Q>\n";
        }
        MPI_Finalize();
        return 1;
    }

    //get the CLI arguments
    int M = std::atoi(argv[1]);
    int Q = std::atoi(argv[2]);

    if (M <= 0 || Q <= 0 || nprocs % Q != 0) {
        if (rank == 0) {
            std::cerr << "Error: M and Q must be positive and Q must divide nprocs evenly.\n";
        }
        MPI_Finalize();
        return 1;
    }


    MPI_Finalize();
    return 0;
}
