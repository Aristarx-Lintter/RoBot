#include "ros/ros.h"
#include "geometry_msgs/Twist.h"


int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "driver");

  ros::NodeHandle n;

  ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  ros::Duration(0.2).sleep();

  geometry_msgs::Twist msg;
  
  std::cout << "Init done\n";
  
  msg.linear.x = 0.1;
  msg.angular.z = 0.1;
  

  vel_pub.publish(msg);
  //ros::spinOnce();
  std::cout << "msg sent\n";
  ros::Duration(2).sleep();
  
  msg.linear.x = 0;
  msg.angular.z = 0;
  
  vel_pub.publish(msg);
  
  ros::Duration(5).sleep();
  
  ros::spinOnce();
  

  //loop_rate.sleep();

  return 0;
}
