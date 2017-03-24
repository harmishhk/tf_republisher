#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

int main(int argc, char **argv) {
  ros::init(argc, argv, "tf_republisher");

  ros::NodeHandle node;
  tf::TransformListener listener;
  ros::Publisher broadcaster = node.advertise<tf::tfMessage>("tf_repub", 100);

  ros::Rate rate(20.0);

  tf::tfMessage filtered_tfs;

  while (node.ok()) {
    try {
      filtered_tfs.transforms.clear();

      tf::StampedTransform tf;
      geometry_msgs::TransformStamped msgtf;

      listener.lookupTransform("/map", "/odom_combined", ros::Time(0), tf);
      tf::transformStampedTFToMsg(tf, msgtf);
      filtered_tfs.transforms.push_back(msgtf);

      listener.lookupTransform("/odom_combined", "/base_footprint", ros::Time(0), tf);
      tf::transformStampedTFToMsg(tf, msgtf);
      filtered_tfs.transforms.push_back(msgtf);

      listener.lookupTransform("/map", "/optitrack", ros::Time(0), tf);
      tf::transformStampedTFToMsg(tf, msgtf);
      filtered_tfs.transforms.push_back(msgtf);
    } catch (tf::TransformException ex) {
      ROS_ERROR("%s", ex.what());
      ros::Duration(1.0).sleep();
    }

    broadcaster.publish(filtered_tfs);

    rate.sleep();
  }

  return 0;
};
