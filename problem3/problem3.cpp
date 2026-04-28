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

/**********************************************************************
    * p - which process owns this element
    * i - the local index inside the process 
    * 
    * p =  6 % 4 = 2  (process 2 will own the element with global index 6)
    * i =  6 / 4 = 1  (local index 1))
    **********************************************************************/
static void scatterDist(int I, int P, int &p, int &i) {
    p = I % P; //which process owns the element I 
    i = I / P; //the local index inside the process
}


//function to find the global index for each row 
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

    //check for the correct number of arguments 
    if (argc != 3) {
        if (rank == 0) {
            std::cerr << "Usage: ./problem3 <M> <Q>\n";
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

    //num. of ranks for each group
    int P = nprocs / Q; 

    // get the row number for the current rank
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
    int y_local_size = partitionSize(M, Q, col); //dist across Q columns 

    //create local vectors for each rank  
    std::vector<double> x(local_size);
    
    //vector y is going to be scattered across Q columns
    std::vector<double> y(y_local_size, 0.0);

    //create the vectors needed for the gather
    std::vector<double> data(M);
    std::vector<int> counts;
    std::vector<int> displs;;

    //init the local vector on rank 0 
    if (rank == 0) {
        data.resize(M);
        counts.resize(P);
        displs.resize(P);

        for (int i = 0; i < M; i++) {
            //init the data vector with incremental valuse [1..M]
            data[i] = i + 1;
        }
        int offset = 0;
        for (int r = 0; r < P; r++) {
            counts[r] = partitionSize(M, P, r);
            displs[r] = offset;
            offset += counts[r];
        }
    }

    //scatter the data vertically down the column 
    if (col == 0) {
    if (rank == 0) {
        // Rank 0 keeps row 0 chunk
        for (int j = 0; j < counts[0]; j++) {
            x[j] = data[displs[0] + j];
        }

        // Send chunks to the other ranks in column 0
        for (int r = 1; r < P; r++) {
            int dest_rank = r * Q;   // rank at grid position (r, 0)
            int count = counts[r];
            int start = displs[r];

            MPI_Send(&count, 1, MPI_INT, dest_rank, 0, MPI_COMM_WORLD);
            MPI_Send(&data[start], count, MPI_DOUBLE, dest_rank, 1, MPI_COMM_WORLD);
        }
    } else {
        int recv_count = 0;

        MPI_Recv(&recv_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(x.data(), recv_count, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}
    MPI_Bcast(x.data(), local_size, MPI_DOUBLE, 0, row_rank);

    //copy the local vector x to y using scatter dist
    //get the global start index
    int start_index = linearStartIndex(M, P, row);  
    for (int j = 0; j < local_size; j++) {
        int k = start_index + j; //global index for the element in x
        int owner_col; 
        int local_index;
        scatterDist(k, Q, owner_col, local_index); 

        if (owner_col == col) {
            y[local_index] = x[j];
        }    
    }

    MPI_Barrier(MPI_COMM_WORLD);

    //print the local vector y for each rank 
    for (int r = 0; r < nprocs; r++) {
        if (rank == r) {
            std::cout << " Rank " << rank << " row: " << row << " col: " << col << " local y: ";
            
            for (double val : y) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Comm_free(&row_rank);
    MPI_Comm_free(&col_rank);

    MPI_Finalize();
    return 0;
}
