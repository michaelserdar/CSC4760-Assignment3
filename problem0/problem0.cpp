#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <vector> 


/************************************ 
    I = Global Index
    N = Vector Length 
    P = Processes (nprocs)
    p = which process owns element I 
    i = local index
    L = base elements 
    R = remainder elements
************************************/
static void forwardLinearMapping(int P, int N, int I, int &p, int &i) {
    //number of entries per partition and the remainers in R
    int L = N / P;
    int R = N % P;

    //vector length is evenly divisible by P 
    if (R == 0) {
        p = I / L;
        i = I % L;
    }else{
        if (I < (L + 1) * R) {
            p = I / (L + 1);
            i = I - (L + 1) * p;
        }else {
            p = R + (I - (L + 1) * R) / L;
            i = I - R * (L + 1) - (p - R) * L;
        }
    }
}

//function to decide on how many elements each rank r gets 
static void partitionSize(int N, int P, int r) {
    int L = N / P;
    int R = N % P;
    return L + (r < R ? 1 : 0);
}

int main(int argc, char **argv){



    return 0;
}