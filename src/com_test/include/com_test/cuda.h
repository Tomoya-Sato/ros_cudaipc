#ifndef CUDA_H
#define CUDA_H

class GpuIpcTest
{
    public:

    GpuIpcTest();
    unsigned char *getHandleBuffer();
    void printDeviceMemory();
    void freeResources();
    void initGpuMemory();
    void freeHandleBuffer();


    private:

    char *data;
    unsigned char *handle_buffer;

};

#endif
