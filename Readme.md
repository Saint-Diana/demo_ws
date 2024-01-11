# 简易双路摄像头数据采集加回放
## 启动摄像头
启动当前采集的双路摄像头节点:使用usb_cam软件包中提供的usb_cam_node节点来获取摄像头数据，摄像头参数在/config文件夹下。
## 同步
同步的过程就是先订阅上面摄像头节点发布的话题，然后进行时间同步，再将同步完成的摄像头数据发布到同步后的话题。
## 录制
启动录制节点:使用rosbag_node软件包中提供的rosbag_record_node节点来订阅同步后发布的摄像头数据，并且保存到~/workspace/ros/RosbagFolder/camera.bag文件中。
## 回放
启动回放节点：使用rosbag_node软件包中提供的rosbag_play_node节点读取~/workspace/ros/RosbagFolder/camera.bag文件，然后将录制的话题再进行发布。
## RVIZ播放采集到的视频
启动rviz，添加两个Image，topic分别设置为/usb_cam1/image_raw和/usb_cam2/image_raw，即可看到上面回放节点播放的视频。