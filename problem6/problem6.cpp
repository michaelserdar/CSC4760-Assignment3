#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>

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

    if (argc != 3) {
        if (rank == 0) {
            std::cerr << "Usage: ./problem6 <M> <Q>\n";
        }
        MPI_Finalize();
        return 1;
    }

    //get the CLI arguments
    int M = std::atoi(argv[1]);
    int Q = std::atoi(argv[2]);

    //check for valid inputs
    if (M <= 0 || Q <= 0 || nprocs % Q != 0) {
        if (rank == 0) {
            std::cerr << "Error: M and Q must be positive, and Q must divide nprocs evenly.\n";
        }
        MPI_Finalize();
        return 1;
    }

     //num. of ranks for each group
    int P = nprocs / Q; 

    //get the row number for the current rank
    int row = rank / Q;

    //get the column number for the current rank 
    int col = rank % Q;

    //row and column communicators for the split
    MPI_Comm row_rank;
    MPI_Comm col_rank;

    //split the comm 
    MPI_Comm_split(MPI_COMM_WORLD, row, rank, &row_rank);
    MPI_Comm_split(MPI_COMM_WORLD, col, rank, &col_rank);

    //get the partition size 
    int local_size = partitionSize(M, P, row); //dist across P rows


    //create local vectors for each rank
    std::vector<double> x(local_size);
    std::vector<double> y(local_size);

    //create global vectors for the full data
    std::vector<double> global_x(M);
    std::vector<double> global_y(M);
    std::vector<int> counts(P);
    std::vector<int> displs(P);


    if (rank == 0) {
        counts.resize(P);
        displs.resize(P);

        //fill the vectors with [1...M] 
        for (int i = 0; i < M; i++) {
            global_x[i] = i + 1.0;
            global_y[i] = i + 1.0; 
        }

        int offset = 0; 
        for (int r = 0; r < P; r++) {
            counts[r] = partitionSize(M, P, r);
            displs[r] = offset;
            offset += counts[r];
        }
    }

    if (col == 0) { 
        if (rank == 0) {
            for (int i = 0; i < counts[0]; i++) {
                x[i] = global_x[displs[0] + i];
                y[i] = global_y[displs[0] + i];
            }

            for (int r = 1; r < P; r++) {
                int dest_rank = r * Q;
                int count = counts[r];
                int start = displs[r];
                
                //send the number of elements to the dest rank for size of the partition
                //not really necessary in this casse as x and y set with partitionSize but good to have   
                MPI_Send(&count, 1, MPI_INT, dest_rank, 0, MPI_COMM_WORLD);

                //send data to the destination rank
                MPI_Send(&global_x[start], count, MPI_DOUBLE, dest_rank, 1, MPI_COMM_WORLD);
                MPI_Send(&global_y[start], count, MPI_DOUBLE, dest_rank, 2, MPI_COMM_WORLD);
            }
        }else {
            int recv_count = 0;
            //get the number of elements for the partition size (x and y should be the same size)
            MPI_Recv(&recv_count, 1, MPI_INT, 0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Recv(x.data(), recv_count, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(y.data(), recv_count, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    //Broadcast across the rows(horizontally)so each rank in the same row has the same data for both vectors 
    MPI_Bcast(x.data(), local_size, MPI_DOUBLE, 0, row_rank);
    MPI_Bcast(y.data(), local_size, MPI_DOUBLE, 0, row_rank);

    double local_dot_product = 0.0;
    double global_dot_product = 0.0;

    //Calc. the local dot product easy since each rank has the same data in the same row
    if (col == 0) {
        for (int i = 0; i < local_size; i++) {
            local_dot_product += x[i] * y[i];
        }
    }

    MPI_Allreduce(&local_dot_product, &global_dot_product, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);


    for (int r = 0; r < nprocs; r++) {
        //print the local and global dot product for the ranks 
        if (rank == r) {
            std::cout << "Rank " << rank << " row: " << row << " col: " << col << " Local dot product: " << 
                local_dot_product << " | Global dot product: " << global_dot_product << "\n";
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Comm_free(&row_rank);
    MPI_Comm_free(&col_rank);


    MPI_Finalize();
    return 0;
}