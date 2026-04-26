#include <mpi.h> 
#include <iostream> 
#include <cstdlib>
#include <vector> 


int main(int argc, char **argv){

    MPI_Init(&argc, &argv);
    int rank, int nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    
    //check for the correct number of arguments
    if (argc != 2) {
        if (rank == 0){
            fprintf(stderr, "Usage: ./problem1 <Q>\n");
        }
        MPI_Finalize();
        return 1;
    }

    //number of queries
    int Q = std::atoi(argv[1]); 

    if (Q <= 0 || nprocs % Q != 0) {
        if (rank ==o) {
            fprintf(stderr, "Error: Q must be positive and divide nprocs evenly.\n");
        }
        MPI_Finalize();
        return 1;
    }

    //number of processes per query 
    int P = nprocs / Q;

    if (rank == 0) {
        std::cout << "Total processes: " << nprocs << "\n";
        std::cout << "Number of queries: " << Q << "\n";
        std::cout << "Processes per query: " << P << "\n";

        for (int r = 0; r < P; r++) {
            std::cout << "Row " << r << ": ";
            for (int c = 0; c < Q; c++) {
                std::cout << r + c * P << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    MPI_Barrier(MPI_COMM_WORLD);

    //split the communicator for rows
    int color1 = rank / P; //rows 
    MPI_Comm row;
    MPI_Comm_split(MPI_COMM_WORLD, color1, rank, &row);

    int row_rank;
    int row_nprocs;
    MPI_Comm_rank(row, &row_rank);
    MPI_Comm_size(row, &row_nprocs);

    int row_sum = 0;

    MPI_Reduce(&rank, &row_sum, 1, MPI_INT, MPI_SUM, 0, row);

    if (row_rank == 0) {
        std::cout << "Sum for row " << color1 << ": " << row_sum << "\n";
    }
    MPI_Barrier(MPI_COMM_WORLD);

    //second communicatorsplit for columns
    int color2 = rank % P; 
    MPI_Comm cols;
    MPI_Comm_split(MPI_COMM_WORLD, color2, rank, &cols);

    int cols_rank;
    int cols_nprocs;
    MPI_Comm_rank(cols, &cols_rank);
    MPI_Comm_size(cols, &cols_nprocs);

    int root_rank = -1;

    if (cols_rank == 0) {
        root_rank = rank;
    }

    MPI_Bcast(&root_rank, 1, MPI_INT, 0, cols);

    //print the column info for each process
    std::cout << "Rank " << rank << " is in column " << color2 << " with root " << root_rank << "\n";  

    MPI_Comm_free(&row);
    MPI_Comm_free(&cols);

    MPI_Finalize();
    return 0;
}