#include <Cuda/test.cuh>
#include <iostream>
#include <cuda_runtime.h>

__global__ void test() {
    printf("Hello World from GPU\n");
}

void testCuda() {
    // Launch kernel
    test<<<1,1>>>();
    printf("Hello world from CPU\n");
}
