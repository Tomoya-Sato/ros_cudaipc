#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "com_test/gpu_handle.h"
#include "com_test/mod.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

unsigned char buf[65];

void chatterCallback(const com_test::gpu_handle msg) {
    printf("receive msg\n");
    modifyDeviceMemory(msg);
}

int main(int argc, char *argv[]) {
    
    ros::init(argc, argv, "sub_node1");
    ros::NodeHandle n;
    
    ros::Subscriber sub = n.subscribe("gpu_handler", 10, chatterCallback);

    ros::spin();

    return 0;
}
