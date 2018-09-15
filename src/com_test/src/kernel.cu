#include "com_test/cuda.h"
#include <stdio.h>
#include <sstream>
#include <string.h>

#include "com_test/common.h"

#define DSIZE 256
inline void gassert(cudaError_t err_code, const char *file, int line)
{
    if (err_code != cudaSuccess) {
        fprintf(stderr, "Error: %s %s %d\n", cudaGetErrorString(err_code), file, line);
        cudaDeviceReset();
        exit(EXIT_FAILURE);
    }
}

#define checkCudaErrors(val) gassert(val, __FILE__, __LINE__)

GpuIpcTest::GpuIpcTest()
{
    return;
}

extern "C" __global__ void gpuMemorySet(int *data, int num)
{
    int id = threadIdx.x + blockDim.x * blockIdx.x;
    int stride = blockDim.x * gridDim.x;

    for (int i = id; i < num; i+=stride) {
        data[i] = i;
    }
}

unsigned char* GpuIpcTest::get_handle_buffer()
{
    cudaIpcMemHandle_t my_handle;
    handle_buffer = (unsigned char*)malloc(sizeof(my_handle)+1);

    char str[DSIZE];
    scanf("%s", str);

    checkCudaErrors(cudaMalloc((void**)&data, DSIZE*sizeof(char)));
    checkCudaErrors(cudaMemcpy(data, str, DSIZE*sizeof(char), cudaMemcpyHostToDevice));

    char tmp[DSIZE];
    checkCudaErrors(cudaMemcpy(tmp, data, DSIZE*sizeof(char), cudaMemcpyDeviceToHost));
    
    checkCudaErrors(cudaIpcGetMemHandle(&my_handle, data));

    unsigned char *buf = (unsigned char*)malloc(sizeof(unsigned char)*(sizeof(my_handle)+1));
    memset(handle_buffer, 0, sizeof(my_handle)+1);
    memcpy(handle_buffer, (unsigned char*)(&my_handle), sizeof(my_handle));

    memcpy(buf, handle_buffer, sizeof(my_handle));

    return buf;
}

void GpuIpcTest::printDeviceMemory()
{
    char tmp[DSIZE];
    checkCudaErrors(cudaMemcpy(tmp, data, sizeof(char)*DSIZE, cudaMemcpyDeviceToHost));

    printf("%s\n", tmp);
}

void GpuIpcTest::free_resources()
{
    checkCudaErrors(cudaFree(data));
    free(handle_buffer);
    
    return;
}
