
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include "geometry_msgs/Twist.h"
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/Quaternion.h>
#include <math.h> 

#include <vector>

//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iomanip>

//файл для калибровки акселерометра
std::ofstream f;

//float secs, nsecs;
long double tim, prev_time;                   //время, прошлое время, приращение
float  dt, pdt=0;

int cal = 50;

float cur_x=0, cur_y=0, prev_x, prev_y;         // текущие х и у
float vx, vy, ax, ay, pvx, pvy, pax, pay;   // скорость, ускорение, прошлая скорость и прошлое ускорение
//float calib_ax=-0.15, calib_ay=0.03, calib_az=-0.21;

float calib_ax, calib_ay, calib_az;

std::vector<float> filter_vector_x;
std::vector<float> filter_vector_y;
std::vector<float> filter_vector_z;


void imu_cb(const sensor_msgs::Imu::ConstPtr& imu_msg){
    static int calib_counter = 0;
    static float sum_x, sum_y, sum_z;
    
    
    if (calib_counter<cal){
        calib_counter++;
        
        if (calib_counter == 1){
                     std::cout<<"\t COLLECTING "<<calib_counter<<std::endl;
         }
    
        filter_vector_x.emplace_back(imu_msg->linear_acceleration.x);
        
        filter_vector_y.emplace_back(imu_msg->linear_acceleration.y);

        filter_vector_z.emplace_back(imu_msg->linear_acceleration.z);

        if (calib_counter == cal){
                     std::cout<<"\t COLLECTED "<<calib_counter<<std::endl;
         }

        return;
    }
    
    if (calib_counter==cal)
    {
//         prev_time=ros::Time::now().toSec()+ros::Time::now().toNSec()*pow(10,-9);
        prev_time=ros::Time::now().toSec();


        std::cout<<"_____________________________________\n";

        std::cout<<"Time in seconds"<<prev_time<<std::endl;
        std::cout<<"_____________________________________\n";
        calib_counter++;
        for (int i=0; i<cal; i++){
        
        sum_x += filter_vector_x[i];
        sum_y += filter_vector_y[i];
        sum_z += filter_vector_z[i];
        
        }
        calib_ax = sum_x/cal;
        calib_ay = sum_y/cal;
        calib_az = sum_z/cal;
        std::cout<<"DONE"<<std::endl;
        
        //f.open("~/catkin_ws/src/publisher_proj/src/accel_calib_data.txt", ios::out);
        f.open("~/catkin_ws/src/publisher_proj/src/accel_calib_x.txt");
        
        for (int i=0; i<cal; i++){
            f<<filter_vector_x[i]<<std::endl;
            std::cout<<filter_vector_x[i]<<std::endl;
        }
        
        f.close();
        
        std::cout<<"File closed"<<std::endl;
        
    }
    
    
    //tim=(imu_msg->header.stamp.sec)+(imu_msg->header.stamp.nsec)/pow(10,9); //время = секунды + наносекунды

//     tim=ros::Time::now().toSec()+ros::Time::now().toNSec()/pow(10,9);
    tim=ros::Time::now().toSec();
    dt=(tim-prev_time);//*pow(10,-6);
    
    std::cout<<tim<<" time"<<std::endl;
    std::cout<<prev_time<<" prev_time"<<std::endl;
    
    std::cout<<dt<<" dt"<<std::endl;
    std::cout<<ros::Time::now()<<" rosTime"<<std::endl;
    

    ax=imu_msg->linear_acceleration.x-calib_ax;      //принимаем ускорения
    ay=imu_msg->linear_acceleration.y-calib_ay;
    
    //vx=ax*dt;
    //vy=ay*dt;

    if ((ax > -0.09 && ax < 0) || ( ax > 0 && ax < 0.09)){
            ax = 0;
    }

    if ((ay > -0.09 && ay < 0) || ( ay > 0 && ay < 0.09)){
            ay = 0;
    }
    
    cur_x+=pax*pdt*dt+(ax*pow(dt,2))/2;//X=X0+Vx*dt+a*(dt^2)/2
    cur_y+=pay*pdt*dt+(ay*pow(dt,2))/2;
    
    prev_time=tim;                          //обновляем прошлое время
    prev_x=cur_x;
    prev_y=cur_y;
    pax=ax;
    pay=ay;
    
    pvx=pax*dt;                             //обновляем прошлоую скорость
    pvy=pay*dt;
    
    pdt=dt;
    
    //p2vx+=pax*dt;
    //p2vx+=pax*dt;
    //ofstream F; 
    std::cout<<"x: "<<cur_x<<std::endl;
    std::cout<<"y: "<<cur_y<<std::endl;
    std::cout<<"ax: "<<ax<<std::endl;
    std::cout<<"ay: "<<ay<<std::endl;
    std::cout<<"--------"<<std::endl;
    
}

//int calib_counter = 0;


int main(int argc, char** argv){
    
    
    ros::init(argc, argv, "imu_position");

    ros::NodeHandle s;
    //ros::NodeHandle s2;
    //ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
    ros::Subscriber odom_sub = s.subscribe("imu", 1000, imu_cb);
    //ros::Subscriber imu_sub = s2.subscribe("imu", 1000, imu_calib_cb);
    
    //tf::TransformBroadcaster odom_broadcaster;
    ros::NodeHandle p;
    ros::Publisher vel_pub = p.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
    
    geometry_msgs::Twist velocity;
    
    

   //filter_vector_x.assign(50,0);
   //filter_vector_y.assign(50,0);
   //filter_vector_z.assign(50,0);
    
    ros::Duration(0.2).sleep();
    ros::spin();
}
//     ros::Rate r(1.0);
//     while(s.ok()){

//         ros::spinOnce();               // check for incoming messages

//         if (theta>1.5708){
//             std::cout<<"right"<<std::endl;
//             velocity.linear.x=0.05;
//             velocity.angular.z=abs(1.5708-theta)*0.2;
//             vel_pub.publish(velocity);
//         }

     //   velocity.linear.x=0.05;
     //   velocity.angular.z=(-theta)*K;
     //   vel_pub.publish(velocity);
        
//         ros::Duration(0.1).sleep();
//         //r.sleep();
//     }
// }