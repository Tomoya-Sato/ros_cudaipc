#include "ros/ros.h"
#include "com_test/cuda.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "com_test/gpu_handle.h"

#include "com_test/common.h"
#include <sstream>
#include <stdio.h>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "gpu_manager");

    ros::NodeHandle n;

    ros::Publisher gpu_pub = n.advertise<com_test::gpu_handle>("gpu_handler", 10);

    GpuIpcTest ins;

    // ros::Rate loop_rate(1);

    // int count = 0;

    data_property ret;

    com_test::gpu_handle msg;
    msg.data.clear();

    unsigned char *buf;

    buf = ins.get_handle_buffer();

    msg.data.clear();
    for (int i = 0; i < 64; i++) {
        msg.data.push_back(buf[i]);
    }
    msg.size = 256;

    gpu_pub.publish(msg);

    int x;

    scanf("%d", &x);

    // while (ros::ok()) {
    //     buf = ins.get_handle_buffer();
    //     ins.printDeviceMemory();

    //     msg.data.clear();
    //     for (int i = 0; i < 64; i++) {
    //         msg.data.push_back(buf[i]);
    //     }
    //     msg.size = 256;

    //     gpu_pub.publish(msg);
    // 
    //     int x;
    //     scanf("%d", &x);

    //     scanf("%d", &x);
    //     printf("break\n");

    //     break;
    // }

   
    return 0;
}
