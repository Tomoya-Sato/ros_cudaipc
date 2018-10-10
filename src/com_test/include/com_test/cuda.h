#ifndef CUDA_H
#define CUDA_H

class GpuIpcTest
{
    public:

    GpuIpcTest();
    void initGpuMemory();
    unsigned char *getHandleBuffer();
    void printDeviceMemory();
    void freeResources();
    void freeHandleBuffer();
    void storeBuffer(const char *str, const int size);
    int getSize();
    bool notEmpty();


    private:

    char *data;
    unsigned char *handle_buffer;
    char *buf[10];
    int sizeBuffer[10];
    int bufCounter;
};

#endif
