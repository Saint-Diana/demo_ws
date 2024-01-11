/**
 * @file rosbag_play_node.cpp
 * @author your name (you@domain.com)
 * @brief 使用rosbag进行回放的节点
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
    std::vector<std::string> playback_topics = {"/usb_cam1/image_raw", "/usb_cam2/image_raw"};
    std::string bag_filename = "/home/shenhuichang/workspace/ros/RosbagFolder/camera.bag";
    RosbagUtils::playRosbag(bag_filename, playback_topics);
    return 0;
}