#include <multi_lidar_calibration/multi_lidar_calibration.h>

sensor_msgs::PointCloud2 cloud1;
unsigned int cloud1_counter = 0;
sensor_msgs::PointCloud2 cloud2;
unsigned int cloud2_counter = 0;

void cloud1_cb(const sensor_msgs::PointCloud2ConstPtr& cloud_msg_ptr) {
  if (cloud1_counter == 0) {
    cloud1_counter++;
    ROS_INFO_STREAM("Received first cloud1. Throwing away..");
  } else {
    if (cloud1_counter == 1) {
      cloud1 = *cloud_msg_ptr;
      cloud1_counter++;
      ROS_INFO_STREAM("Received second cloud1.");
    }
  }
}

void cloud2_cb(const sensor_msgs::PointCloud2ConstPtr& cloud_msg_ptr) {
  if (cloud2_counter == 0) {
    cloud2_counter++;
    ROS_INFO_STREAM("Received first cloud2. Throwing away..");
  } else {
    if (cloud2_counter == 1) {
      cloud2 = *cloud_msg_ptr;
      cloud2_counter++;
      ROS_INFO_STREAM("Received second cloud2.");
    }
  }
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "multi_lidar_calibration_node");
  ROS_INFO_STREAM("Multi lidar calibration node started. Waiting for point clouds.");

  ros::NodeHandle nh;
  // google::InitGoogleLogging(argv[0]);

  ros::Subscriber cloud1_sub = nh.subscribe("cloud1", 10, &cloud1_cb);
  ros::Subscriber cloud2_sub = nh.subscribe("cloud2", 10, &cloud2_cb);
  hector_calibration::lidar_calibration::MultiLidarCalibration mlc(nh);

  ros::Rate rate(10);
  while (ros::ok() && (cloud1_counter < 2 || cloud2_counter < 2)) {
    ros::spinOnce();
    rate.sleep();
  }
  ROS_INFO_STREAM("Received both point clouds");
  mlc.calibrate(cloud1, cloud2);

  return 0;
}
