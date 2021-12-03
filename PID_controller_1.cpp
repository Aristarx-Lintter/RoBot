#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include <math.h>
#include <vector>


float kp = 1.5;
float kd = 100;
float target = 0.3;
float tmp=0, I=0, err=0;

//float i[50];
std::vector<float> integral;
  	
geometry_msgs::Twist velocity;

float MIN_fun(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	int i;
	float min = scan->ranges[0];
	for(i = 0; i < 359; i++)
	{
		if(min > scan->ranges[i])
			min = scan->ranges[i];
	}
	return min;
}

float sum(float x){
    float result;
    
    integral.emplace(integral.begin(),x);
    integral.pop_back();
 
    for (int i=0; i<50; i++){
        result+=integral.at(i);
    }
    
    return result;
}

void laser_msg_Callback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
  	float m = MIN_fun(scan);
  	err = target - m;
  	//I = I + res;
            I=sum(err);
  	velocity.angular.z = kp * err + I * kp*0.02 + kd * (err - tmp);
  	tmp = err;
  	std::cout << m << std::endl;
  	//std::cout<<scan->ranges[0]<<std::endl;
}                  

int main(int argc, char **argv)
{

  	ros::init(argc, argv, "lidar_listener"); 

  	ros::NodeHandle n1;
  	ros::Subscriber sub = n1.subscribe("scan", 1000, laser_msg_Callback);

  	ros::NodeHandle n2;
  	ros::Publisher vel_pub = n2.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  	

  	ros::Duration(0.2).sleep();

  	velocity.linear.x = 0.1;

  	vel_pub.publish(velocity);
    
    
//     for (int a=0; a<50; a++){
//         i[a]=0;
//     }
    
    integral.assign(50,0);

  	while(ros::ok())
  	{
  		vel_pub.publish(velocity);

  		ros::spinOnce();
  		ros::Duration(0.2).sleep();
  	}


  	return 0;
}