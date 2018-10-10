#include "ros/ros.h"
#include "com_test/cuda.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "com_test/gpu_handle.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"

#include "com_test/common.h"
#include <sstream>
#include <stdio.h>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static ros::Publisher gpu_pub;

static GpuIpcTest ins;

com_test::gpu_handle send_msg;
unsigned char *buf;

static int count = 0;

const int node_count = 1;
static int free_count = 1;


void publishHandle() {
    send_msg.size = ins.getSize();
    buf = ins.getHandleBuffer();

    send_msg.data.clear();
    for (int i = 0; i < 64; i++) {
        send_msg.data.push_back(buf[i]);
    }

    gpu_pub.publish(send_msg);

    free(buf);
    free_count = 0;

    return;
}

void lidar_callback(const std_msgs::String::ConstPtr &msg) {
    ROS_INFO("Receive msg from Pseudo_Lidar.");
    ins.storeBuffer(msg->data.c_str(), msg->data.length());
    
    if (node_count == free_count) publishHandle();

    return;
}

void free_callback(std_msgs::Bool msg) {
    ROS_INFO("Receive free signal.");
    free_count++;

    if ((node_count == free_count) && (ins.notEmpty())) publishHandle();

    return;
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "gpu_manager");

    ros::NodeHandle n;

    gpu_pub = n.advertise<com_test::gpu_handle>("gpu_handler", 10);

    ins.initGpuMemory();

    ros::Subscriber lidar_sub = n.subscribe("lidar_data", 10, lidar_callback);
    ros::Subscriber free_sub = n.subscribe("free_signal", 10, free_callback);

    ros::spin();

    ins.freeResources();

    return 0;
}
