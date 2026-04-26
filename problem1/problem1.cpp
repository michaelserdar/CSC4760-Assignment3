#include <mpi.h> 
#include <iostream> 
#include <cstdlib>
#include <vector> 


int main(int argc, char **argv){

    MPI_Init(&argc, &argv);
    int rank, int nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    



    MPI_Finalize();
    return 0;
}