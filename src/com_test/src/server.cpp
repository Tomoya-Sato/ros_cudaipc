#include "ros/ros.h"
#include "com_test/gpuInfo.h"
#include "com_test/mod.h"

unsigned char buf[65];

bool chatterCallback(com_test::gpuInfo::Request &req, com_test::gpuInfo::Response &res) {
    ROS_INFO("receive devPtr: %s", req.devPtr);
    res.result = modifyDeviceMemory(req);

    return true;
}

int main(int argc, char *argv[]) {
    
    ros::init(argc, argv, "server");
    ros::NodeHandle n;
    
    ros::ServiceServer service = n.advertiseService("gpu_info", chatterCallback);

    ROS_INFO("Ready to receive.");
    ros::spin();

    return 0;
}
