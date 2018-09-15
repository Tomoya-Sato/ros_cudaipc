#ifndef CUDA_H
#define CUDA_H

class GpuIpcTest
{
    public:

    GpuIpcTest();
    unsigned char *get_handle_buffer();
    void printDeviceMemory();
    void free_resources();


    private:

    char *data;
    unsigned char *handle_buffer;

};

#endif
