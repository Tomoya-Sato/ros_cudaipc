#include "ros/ros.h"
#include "com_test/cuda.h"
#include "com_test/gpuInfo.h"

#include "com_test/common.h"
#include <sstream>
#include <stdio.h>

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "gpu_manager");

    ros::NodeHandle n;

    ros::ServiceClient client = n.serviceClient<com_test::gpuInfo>("gpu_info");

    GpuIpcTest ins;

    com_test::gpuInfo srv;

    unsigned char *buf;

    ins.initGpuMemory();

    while (ros::ok()) {
        buf = ins.getHandleBuffer();

        srv.request.devPtr.clear();
        for (int i = 0; i < 64; i++) {
            srv.request.devPtr.push_back(buf[i]);
        }
        srv.request.size = 256;

        if (client.call(srv)) {
            ROS_INFO("receive true.");
        }
        else {
            ROS_INFO("receive false.");
        }
        
        free(buf);
    }

    return 0;
}
