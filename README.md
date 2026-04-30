# CSC4760-Parallel Computing 
## Homework 3: MPI and CUDA Programming 

### This repo exists to hold my implementation of the CSC4760 Homework 3 problems 0-6. The problems covers concepts for index mapping and partitioning, communicators, data distribution, CUDA matrix addition, Floating point precision, and Parallel dot product calculations. 

---

### Project Structure 
```text
Homework3/
├── problem0/ 
├── problem1/ 
├── problem2/ 
├── problem3/ 
├── problem4/ 
├── problem5/ 
├── problem6/ 
├── Makefile
└── README.md
```

--- 

### Build Instructions 

Using the included Makefile 
```bash 
make
```

Cleaning the build artifacts: 
```bash
make clean
```

### Running the Programs 
MPI Programs (1-3 and 6)
```bash 
mpirun -np 8 problem1/problem1 16 4
```

CUDA Program 
```bash 
srun --account=bfrm-delta-gpu \
     --partition=gpuA100x4-interactive \
     --gres=gpu:1 \
     --ntasks=1 \
     --pty bash

make problem4
./problem4
```

--- 

### Problem 0 
You should do this problem first, as it will refresh your knowledge on linear and scatter mappings and help
you with the next 3 problems immensely.
Imagine that you have a vector v, of length M . You have P processes, and the vector is distributed
among the processes twice—first as a linear mapping, then as a scatter mapping. This exercise is about
global-to-local and local-to-global mappings in either linear or scattered distributions.
Note: In later problems, you will have such vectors distributed and replicated on process topologies of
shape P × Q, but here we are just recalling how to map between local and global indices and processes in
both mappings.
You have P processes, a vector v of length M , and youh ave the process rank value as p, with
index in that process i. Find the global index I of that triple (p, i, M ) with a linear inverse
distribution.
Given that same global index I, the length of the array M , and the number of processes P , is
it possible to find the rank of the process p, and the local index i of its scatter-mapped index?
How?
Write a sequential program that takes M , P , p, and i (for the linear-mapped distribution), and
returns the global index I. Then, compute and the process rank p′ and local index i′ that it
maps to on the scatter mapping. You do not need to actually allocate or distribute an array,
just do the indexing, but it may be helpful to do so. Make sure to handle cases where the
array does not divide evenly into the processes.
See the figure 1 for an example where P = 4,
M = 8, looking at local index i = 1 in the linear chunk of process p = 0. Look at the linear mapping code
provided by Dr. Skjellum for more help. Be sure to look at notes and code provided before for forward and
inverse mappings of linear and scattered?

---

### Problem 1
Write a program that uses MPI Comm split to create two distinct sets of sub-communicators for all processes
in MPI COMM WORLD. The world size must be exactly P × Q for P, Q ≥ 1. In the first split, group processes
that have the same color when their ranks are divided by the integer Q; within these groups, perform an
MPI Reduce to calculate the sum of the world ranks and have the root of each sub-communicator print the
result. In the second split, group processes together that have the same color when the color is computed
as rank modulo Q; within these groups, have the root process perform an MPI Bcast of its original world
rank to all other processes in that sub-communicator. Each process should then print the value it received
to demonstrate that the collective communication was isolated to processes of the same color.

---

### Problem 2
Write an MPI program that builds a 2D process topology of shape P ×Q. On each column of processes, store a
vector x of length M , distributed in a linear load-balanced fashion “vertically” (it will be replicated Q times).
Start with data only in process (0,0), and scatter it down the first column. Once it is scattered on column 0,
broadcast it horizontally in each process row. Allocate a vector y of length M that is replicated “horizontally”
in each process row and stored also in linear load-balanced distribution; there will be P replicas, one in each
process row. Using MPI Allreduce or MPI Allgather with the appropriate communicators, do the parallel
copy y := x. There should be P replicas of the answer in y when you’re done.

---

### Problem 3
Modify Problem #2 above, but store y in a scatter distribution (aka wrap-mapped) distribution. For this
case, from global coeffient J on Q processes, then local index is j = J/Q, q = J mod Q, and the number of
elements per process is the same as in the linear load-balanced distribution would produce with N elements
over Q partitions.

---

### Problem 4
Develop a CUDA program to perform row-wise addition of a 1D vector to a 2D matrix. Initialize the host
matrix A as a 3 × 3 grid with the following values:

```text
A = [130, 147, 115, 224, 158, 187, 54, 158, 120]
```
and vector B with values: 
```text
B = [221, 12, 157]
```
Transfer these to device memory and execute a parallel kernel where each thread calculates A[row][col] +
B[col] using flat-index mapping (row × cols + col). Finally, copy the updated matrix back to the host to
verify the result against the expected solution. The expected value of this updated matrix is:

```text
Soln = [351, 159, 272, 445, 170, 344, 275, 170, 277]
```
---

### Problem 5
Using C/C++, add 1E − 18 to the a double variable starting with the value 1.0, 100,000 times. Output the
result in high precision. Next, add the number 1E − 18 to a double variable starting with the value 0, also
100,000 times. Then add 1 to that sum, and compare the results in the two variables by subtracting them,
and also output them in high precision. What’s the difference in these two sums? (For fun, also output
the results of the sums in hexadecimal to see any differences in bit patterns, so you learn how to basically
inspect floating point numbers in hex for debugging.)

---

### Problem 6
Using the results of Problem #1 and #3 to compute the dot product of two distinct vectors of length M , one
stored initially horizontally in linear load-balanced distribution, and one stored initially vertically in linear
load-balanced distribution. The scalar result should be in all processes at the end of the computation.
Hint: You can do the copy operation shown in Problems #2 and #3 to make this much easier.

--- 

### Submission notes 
- All code compiled and tested either locally for MPI or on the Delta cluster for CUDA
- Output varified for correctness across all problems 

--- 

### Author 
Michael Serdar Computer Science - Tennessee Technological Univarsity 
Course: CSC 4760 - Parallel Computing 
