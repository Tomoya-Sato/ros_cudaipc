#include "com_test/mod.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "com_test/gpu_handle.h"
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

void modifyDeviceMemory(com_test::gpu_handle msg)
{

    cudaIpcMemHandle_t my_handle;

    unsigned char buf[64];
    for (int i = 0; i < 64; i++) {
        buf[i] = msg.data[i];
    }

    memcpy((unsigned char*)&my_handle, buf, sizeof(my_handle));

    char *data;

    checkCudaErrors(cudaIpcOpenMemHandle((void**)&data, my_handle, cudaIpcMemLazyEnablePeerAccess));

    char tmp[DSIZE];
    checkCudaErrors(cudaMemcpy(tmp, data, sizeof(char)*DSIZE, cudaMemcpyDeviceToHost));

    printf("%s\n", tmp);

    return;
}
