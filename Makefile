CXX = mpic++
NVCC = nvcc
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

.PHONY: all clean problem0 problem1 problem2 problem3 problem4 problem5 problem6

all: problem0 problem1 problem2 problem3 problem4 problem5 problem6

problem0:
	$(CXX) $(CXXFLAGS) problem0/problem0.cpp -o problem0/problem0

problem1:
	$(CXX) $(CXXFLAGS) problem1/problem1.cpp -o problem1/problem1

problem2:
	$(CXX) $(CXXFLAGS) problem2/problem2.cpp -o problem2/problem2

problem3:
	$(CXX) $(CXXFLAGS) problem3/problem3.cpp -o problem3/problem3

problem4:
	$(NVCC) problem4/problem4.cu -o problem4/problem4

problem5:
	$(CXX) $(CXXFLAGS) problem5/problem5.cpp -o problem5/problem5

problem6:
	$(CXX) $(CXXFLAGS) problem6/problem6.cpp -o problem6/problem6

clean:
	rm -f problem0/problem0
	rm -f problem1/problem1
	rm -f problem2/problem2
	rm -f problem3/problem3
	rm -f problem4/problem4
	rm -f problem5/problem5
	rm -f problem6/problem6
	rm -rf build