#include <cuda_runtime.h>
#include <vector>
#include <iostream> 

int main() {

    // Define the dimensions of the 2D Matrix
    const int rows = 3;
    const int cols = 3;
    const int size = rows * cols; 

    //Define the 2d Matrix 
    std::vector<int> A = {130, 147, 115, 
                          224, 158, 187, 
                          54, 158, 120};

    //Define the 1D array                 
    std::vector<int> B = {221, 12, 157};

    // Allocate memory on the device
    int *d_A;
    int *d_B;

    cudaMalloc(&d_A,size * sizeof(int));
    cudaMalloc(&d_B,cols * sizeof(int));

    //Copy data from host to device 
    cudaMemcpy(d_A, A.data(), size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B.data(), cols * sizeof(int), cudaMemcpyHostToDevice);

    


    return 0;
}