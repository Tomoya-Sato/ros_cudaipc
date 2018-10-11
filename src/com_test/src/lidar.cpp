#include <stdio.h>
#include <sstream>
#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char *argv[]) {

    ros::init(argc, argv, "psuedo_lidar");
    ros::NodeHandle n;

    ros::Publisher lidar_pub = n.advertise<std_msgs::String>("lidar_data", 10);

    char tmp[256];
    std_msgs::String msg;
    std::stringstream ss;

    while (ros::ok()) {
        msg.data.clear();
        ss.str("");
        ss.clear(std::stringstream::goodbit);
        if (scanf("%s", tmp) == EOF) break;
        ss << tmp;
        msg.data = ss.str();

        ROS_INFO("%s", msg.data.c_str());

        lidar_pub.publish(msg);
       
    }

    return 0;
}
