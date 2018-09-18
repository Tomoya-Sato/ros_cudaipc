#include "com_test/mod.h"
#include "com_test/gpuInfo.h"
#include <stdio.h>

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

bool modifyDeviceMemory(com_test::gpuInfo::Request &req)
{

    cudaIpcMemHandle_t my_handle;

    unsigned char buf[64];
    for (int i = 0; i < 64; i++) {
        buf[i] = req.devPtr[i];
    }

    memcpy((unsigned char*)&my_handle, buf, sizeof(my_handle));

    char *data;

    checkCudaErrors(cudaIpcOpenMemHandle((void**)&data, my_handle, cudaIpcMemLazyEnablePeerAccess));

    char *tmp = (char*)malloc(sizeof(char)*req.size);
    checkCudaErrors(cudaMemcpy(tmp, data, sizeof(char)*req.size, cudaMemcpyDeviceToHost));

    printf("gpu memory: %s\n", tmp);

    checkCudaErrors(cudaIpcCloseMemHandle(data));
    free(tmp);

    return true;
}
