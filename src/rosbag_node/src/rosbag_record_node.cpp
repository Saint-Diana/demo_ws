/**
 * @file rosbag_record_node.cpp
 * @author your name (you@domain.com)
 * @brief 使用rosbag进行录制的节点
 * @version 0.1
 * @date 2024-01-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "rosbag_utils.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "rosbag_record_node");
    std::vector<std::string> record_topics = {"/usb_cam1/image_sync", "/usb_cam2/image_sync"};
    std::string bag_filename = "/home/shenhuichang/workspace/ros/RosbagFolder/camera.bag";
    double duration = 20;
    RosbagUtils::recordRosbag(record_topics, bag_filename, duration);
    return 0;
}