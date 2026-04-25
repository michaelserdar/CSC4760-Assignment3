#include <cstdio>
#include <cstdlib>
#include <vector> 


//function prototypes 
static int partitionSize();
static int linearInverseDist();
static void scatterDist();

int main(int argc, char **argv){
    
    //check for correct number of arguments
    if (argc != 5) {
        fprintf(stderr, "Usage: ./problem0 <M> <P> <p> <i>\n");
        return 1;
    }

    int M = std::atoi(argv[1]); //num of elements
    int P = std::atoi(argv[2]); //num of processes
    int p = std::atoi(argv[3]); //rank of the process
    int i = std::atoi(argv[4]); //local index in the process



    return 0;
}

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

    //figure out the starting point for each rank  
    int start;
    if (p < R) {
        start = L + 1;
    } else {
        start = L;
    }

    //return the global index for i in rank p
    return p * L + start + i;
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
