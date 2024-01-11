#include "rosbag_utils.h"

/**
 * @brief 录制rosbag
 *
 * @param topics 需要录制的话题列表
 * @param bag_filename bag文件地址
 * @param duration 录制时长
 */
void RosbagUtils::recordRosbag(const std::vector<std::string> &topics, const std::string &bag_filename, double duration)
{
    ros::NodeHandle nh; /* 创建 ros 节点 */

    rosbag::Bag bag;                                    /* 创建 rosbag::Bag 对象 */
    bag.open(bag_filename, rosbag::bagmode::Write);     /* 写入 bag_filename */

    std::vector<ros::Subscriber> subs;          /* 创建 ros 话题订阅器 */
    for (const auto& topic : topics)
    {
        if (topic.find("/usb_cam") == 0)      /* 如果 topic 以 sub_camera 开头 */
        {
            subs.push_back(nh.subscribe<sensor_msgs::Image>(topic, 1, [&](const sensor_msgs::Image::ConstPtr& msg) {
                /* 回调函数 */
                bag.write(topic, ros::Time::now(), *msg);
            }));
        } /* 预留接口：这里还可以添加其他想要录制的话题 */
        else
        {
            ROS_WARN_STREAM("Unknown topic: " << topic);
        }
    }

    /* 持续录制 */
    if (duration <= 0)
    {
        ROS_INFO_STREAM("Start recording indefinitely...");
        ros::spin();
    }
    else
    {
        ROS_INFO_STREAM("Start recording for " << duration << " seconds...");
        ros::Time start_time = ros::Time::now();
        while ((ros::Time::now() - start_time).toSec() < duration)
        {
            ros::spinOnce();
        }
    }

    /* 停止订阅器 */
    for (auto& sub : subs)
    {
        sub.shutdown();
    }

    bag.close(); /* 关闭 rosbag 文件 */

    ROS_INFO_STREAM("Recording finished!");
}

/**
 * @brief 回放rosbag
 *
 * @param bag_filename bag文件路径
 * @param topics 需要回放的话题列表
 */
void RosbagUtils::playRosbag(const std::string &bag_filename, std::vector<std::string> &topics)
{
    ros::NodeHandle nh; /* 创建 ros 节点 */

    if (topics.empty())
    {
        ROS_INFO_STREAM("Topics is empty!");
        return;
    }

    /* 创建 ros 话题发布器 */
    std::vector<ros::Publisher> pubs;
    for (const auto& topic : topics)
    {
        if (topic.find("/usb_cam") == 0)
        {
            pubs.push_back(nh.advertise<sensor_msgs::Image>(topic, 1));
        }
        else
        {
            ROS_WARN_STREAM("Unknown topic: " << topic);
        }
    }

    /* 回放数据 */
    rosbag::Bag bag;                                        /* 创建 rosbag::Bag 对象 */
    bag.open(bag_filename, rosbag::bagmode::Read);          /* 读取 bag_filename */

    /* 使用迭代器遍历，删除不以"/usb_cam"开头的话题 */
    for (auto it = topics.begin(); it != topics.end(); ) {
        if (it->compare(0, 8, "/usb_cam") != 0) {
            it = topics.erase(it);
        } else {
            ++it;
        }
    }

    rosbag::View view(bag, rosbag::TopicQuery(topics));     /* 遍历 rosbag */

    /* 获取 ROS bag 文件的录制时长 */
    ros::Time start_time = view.getBeginTime();
    ros::Time end_time = view.getEndTime();
    double duration = (end_time - start_time).toSec();

    /* 设置回放参数 */
    rosbag::PlayerOptions options;
    options.bags.push_back(bag_filename);           /* 设置回放的 bag 路径 */
    options.topics = topics;                        /* 设置回放的话题 */
    options.has_duration = true;                    /* 设置允许回放一段时间 */
    options.duration = duration;                    /* 设置回放时长 */
    rosbag::Player player(options);
    player.publish();

    /* 关闭 rosbag */
    bag.close();

    ROS_INFO_STREAM("Playblack finished!");
}