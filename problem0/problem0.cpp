#include <cstdio>
#include <cstdlib>
#include <vector> 
#include <iostream> 


//function to decide on how many elements each rank r gets 
static int partitionSize(int M, int P, int p) {
    int L = M / P;
    int R = M % P;
    return L + (p < R ? 1 : 0);
}
//function to find the global index using inverse linear distibution
static int linearInverseDist(int M, int P, int p, int i) {
    int L = M / P;
    int R = M % P;

    //cout how many elements are in each rank
    int extra;
    if (p < R) {
        extra = p;
    } else {
        extra = R;
    }

    //return the global index for i in rank p
    return p * L + extra + i;
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


int main(int argc, char **argv){
    
    //check for correct number of arguments
    if (argc != 5) {
        fprintf(stderr, "Usage: ./problem0 <M> <P> <p> <i>\n");
        return 1;
    }

    //get the CLI arguments
    int M = std::atoi(argv[1]); //num of elements
    int P = std::atoi(argv[2]); //num of processes
    int p = std::atoi(argv[3]); //rank of the process
    int i = std::atoi(argv[4]); //local index in the process

    //check for valid inputs 
    if (M <= 0 || P <= 0) {
        fprintf(stderr, "M and P must be positive.\n");
        return 1;
    }

    if (p < 0 || p >= P) {
        fprintf(stderr, "p is out of bounds.\n");
        return 1;
    }

    int localSize = partitionSize(M, P, p);

    if (i < 0 || i >= localSize) {
        fprintf(stderr, "i is out of bounds.\n");
        return 1;
    }

    //get the global index (I) for the local index (i) in process (p)
    int I = linearInverseDist(M, P, p, i);
    int p_prime = 0;
    int i_prime = 0;

    //get the process rank (p) and local index (i) for the global index (I)
    scatterDist(I, P, p_prime, i_prime);

    //print the results 
    std::cout << "Given M = " << M << " P = " << P << "\n";
    std::cout << "local size for process " << p << ": " << localSize << std::endl;
    std::cout << "----------------------------------------\n\n";

    //linear mapping results
    std::cout << "Linear Distribution: \n"; 
    std::cout << "process: " << p << " index: " << i << " Global Index: " << I << std::endl;

    std::cout << "\n----------------------------------------\n\n";

    //scatter mapping results
    std::cout << "Scatter Distribution: \n";
    std::cout << "Global Index: " << I << " process p': " << p_prime << " local index i': " << i_prime << std::endl;

    return 0;
}

