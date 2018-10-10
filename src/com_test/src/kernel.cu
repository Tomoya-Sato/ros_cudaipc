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
    bufCounter = 0;
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

void GpuIpcTest::initGpuMemory()
{
    cudaMalloc((void**)&data, DSIZE*sizeof(char));

    return;
}

unsigned char* GpuIpcTest::getHandleBuffer()
{
    cudaIpcMemHandle_t my_handle;
    handle_buffer = (unsigned char*)malloc(sizeof(my_handle)+1);

    checkCudaErrors(cudaMemcpy(data, buf[0], sizeBuffer[0]*sizeof(char), cudaMemcpyHostToDevice));

    checkCudaErrors(cudaIpcGetMemHandle(&my_handle, data));

    memset(handle_buffer, 0, sizeof(my_handle)+1);
    memcpy(handle_buffer, (unsigned char*)(&my_handle), sizeof(my_handle));

    free(buf[0]);
    for (int i = 0; i < bufCounter; i++) {
        buf[i] = buf[i+1];
        sizeBuffer[i] = sizeBuffer[i+1];
    }

    bufCounter--;

    return handle_buffer;
}

void GpuIpcTest::storeBuffer(const char *str, const int size)
{
    if (bufCounter == 10) {
        fprintf(stderr, "pointcloud buffer overflow.\n");
        exit(EXIT_FAILURE);
    }
    buf[bufCounter] = (char*)malloc(sizeof(char)*size);
    memcpy(buf[bufCounter], str, sizeof(char)*size);

    sizeBuffer[bufCounter] = size;

    bufCounter++;
    
    return;
}

int GpuIpcTest::getSize() {
    return sizeBuffer[0];
}

void GpuIpcTest::freeHandleBuffer()
{
    free(handle_buffer);

    return;
}

void GpuIpcTest::printDeviceMemory()
{
    char tmp[DSIZE];
    checkCudaErrors(cudaMemcpy(tmp, data, sizeof(char)*DSIZE, cudaMemcpyDeviceToHost));

    printf("%s\n", tmp);
}

void GpuIpcTest::freeResources()
{
    checkCudaErrors(cudaFree(data));
    
    return;
}

bool GpuIpcTest::notEmpty()
{
    if (bufCounter == 0) return false;
    else return true;
}
