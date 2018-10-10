#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "com_test/gpu_handle.h"
#include "com_test/mod.h"
#include "std_msgs/Bool.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

unsigned char buf[65];

std_msgs::Bool bl1;
static ros::Publisher free_pub;

void chatterCallback(const com_test::gpu_handle msg) {
    ROS_INFO("Receive message.");
    modifyDeviceMemory(msg);
    int x;
    scanf("%d", &x);
    free_pub.publish(bl1);
}

int main(int argc, char *argv[]) {
    
    ros::init(argc, argv, "sub_node1");
    ros::NodeHandle n;
    
    bl1.data = true;

    free_pub = n.advertise<std_msgs::Bool>("free_signal", 10);

    ros::Subscriber sub = n.subscribe("gpu_handler", 10, chatterCallback);

    ROS_INFO("Ready to receive.");
    ros::spin();

    return 0;
}
