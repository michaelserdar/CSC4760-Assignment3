#include <cuda_runtime.h>
#include <vector>
#include <iostream> 


//CUDA kernal for the matrix addition
__global__ void matrixVectorAdd(int *A, int *B, int rows, int cols) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int total = rows * cols;

    if (idx < total) {
        //column index
        int col = idx % cols;
        A[idx] = A[idx] + B[col];

    }
}


int main() {

    //define the dimensions of the 2D Matrix
    const int rows = 3;
    const int cols = 3;
    const int size = rows * cols; 

    //define the 2d Matrix 
    std::vector<int> A = {130, 147, 115, 224, 158, 187, 54, 158, 120};

    //define the 1D array                 
    std::vector<int> B = {221, 12, 157};

    //solution vector 
    std::vector<int> soln(size);
    
    //allocate memory on the device
    int *d_A;
    int *d_B;
    cudaMalloc(&d_A,size * sizeof(int));
    cudaMalloc(&d_B,cols * sizeof(int));

    //Copy data from host to device 
    cudaMemcpy(d_A, A.data(), size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B.data(), cols * sizeof(int), cudaMemcpyHostToDevice);

    //define the block and grid size 
    int threadsPerBlock = 256;
    int blocks = (size + threadsPerBlock -1) / threadsPerBlock;

    //send the kernel to the device 
    matrixVectorAdd<<<blocks, threadsPerBlock>>>(d_A, d_B, rows, cols);

    cudaDeviceSynchronize();

    //copy resuts back to the CPU using the solution vector
    cudaMemcpy(soln.data(), d_A, size * sizeof(int), cudaMemcpyDeviceToHost);


    //print out the resutls 
    std::cout << "Result of Matrix vector addition: " << "\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; i++) {
            std::cout << soln[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }

    cudaFree(d_A);
    cudaFree(d_B); 

    return 0;
}