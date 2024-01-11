#ifndef ROSBAG_UTILS_H
#define ROSBAG_UTILS_H

#include "vector"
#include "string"
#include "ros/ros.h"
#include "rosbag/bag.h"
#include "sensor_msgs/Image.h"
#include "rosbag/view.h"
#include "rosbag/player.h"

class RosbagUtils {
public:
    /* 录制rosbag，可以指定录制的话题，bag文件存储地址以及录制时长 */
    static void recordRosbag(const std::vector<std::string>& topics, const std::string& bag_filename, double duration = 0);
    /* 回放rosbag，可以指定回放的bag文件地址以及回放的话题 */
    static void playRosbag(const std::string& bag_filename, std::vector<std::string>& topics);
};

#endif