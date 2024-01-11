/**
 * @file camera_sync_node.cpp
 * @author your name (you@domain.com)
 * @brief 进行两路摄像头数据的同步
 * @version 0.1
 * @date 2024-01-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "message_filters/subscriber.h"
#include "message_filters/time_synchronizer.h"
#include "message_filters/sync_policies/approximate_time.h"

ros::Publisher pub_cam1;
ros::Publisher pub_cam2;

void callback(const sensor_msgs::ImageConstPtr& image1, const sensor_msgs::ImageConstPtr& image2) {
    /* 这里处理同步后的两个图像数据
       image1 和 image2 分别是两个摄像头发布的图像消息
       需要将同步后的两个图像数据发布到话题/usb_cam1/image_sync和/usb_cam2/image_sync中 */
    pub_cam1.publish(image1);       /* 发布到 /usb_cam1/image_sync */
    pub_cam2.publish(image2);       /* 发布到 /usb_cam2/image_sync */
    ROS_INFO_STREAM("Synchronized data is publishing!");
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "camera_sync_node");
    ros::NodeHandle nh;

    /* 创建两个消息过滤器的订阅者 */
    message_filters::Subscriber<sensor_msgs::Image> sub1(nh, "/usb_cam1/image_raw", 1);
    message_filters::Subscriber<sensor_msgs::Image> sub2(nh, "/usb_cam2/image_raw", 1);

    /* 创建时间同步器，同步两个消息订阅者 */
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> MySyncPolicy;
    message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), sub1, sub2);

    /* 设置回调函数 */
    sync.registerCallback(boost::bind(&callback, _1, _2));

    /* 创建两个图像发布者 */
    pub_cam1 = nh.advertise<sensor_msgs::Image>("/usb_cam1/image_sync", 1);
    pub_cam2 = nh.advertise<sensor_msgs::Image>("/usb_cam2/image_sync", 1);

    ros::spin();

    return 0;
}
