#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include "geometry_msgs/Twist.h"
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/Quaternion.h>
#include <math.h> 

#include <vector>

//float secs, nsecs;
long double tim, prev_time;                   //время, прошлое время, приращение
float  dt;
int k;

float cur_x=0, cur_y=0, prev_x, prev_y, list_x[10];         // текущие х и у
float vx, vy, ax, ay, pvx, pvy, pax, pay;   // скорость, ускорение, прошлая скорость и прошлое ускорение
//float calib_ax=-0.15, calib_ay=0.03, calib_az=-0.21;

float calib_ax, calib_ay, calib_az, sums = 0;

std::vector<float> filter_vector_x;
std::vector<float> filter_vector_y;
std::vector<float> filter_vector_z;

std::vector<float> show_vector_x;
std::vector<float> show_vector_y;
std::vector<float> show_vector_z;


void imu_cb(const sensor_msgs::Imu::ConstPtr& imu_msg){
    static int calib_counter = 0;
    static float sum_x, sum_y, sum_z;
    
    if(calib_counter==0){std::cout<<"COLLECTING"<<std::endl;}
    
    
    
//     if (calib_counter<40){
//         calib_counter++;
    
//         filter_vector_x.emplace_back(imu_msg->linear_acceleration.x);
        
//         filter_vector_y.emplace_back(imu_msg->linear_acceleration.y);

//         filter_vector_z.emplace_back(imu_msg->linear_acceleration.z);
        
//         return;
//     }

     if (calib_counter<100){
        calib_counter++;
    
        filter_vector_x.emplace_back(imu_msg->linear_acceleration.x);
        
        filter_vector_y.emplace_back(imu_msg->linear_acceleration.y);

        filter_vector_z.emplace_back(imu_msg->linear_acceleration.z);
        
        return;
    }

    if (calib_counter==100){

            for (float i: filter_vector_x){
                        sums += i;
                        
            }

            for (float i: filter_vector_x){
                        show_vector_x.emplace_back(i-(sums/100));
                        
            }
            sums=0;


            for (float i: filter_vector_y){
                        sums += i;
            }
            
            for (float i: filter_vector_y){
                        show_vector_y.emplace_back(i-(sums/100));
                        
            }
            sums=0;


            for (float i: filter_vector_z){
                        sums += i;
            }
            for (float i: filter_vector_z){
                        show_vector_z.emplace_back(i-(sums/100));
                        
            }
            sums=0;



            // filter_vector_x.clear();
            // filter_vector_y.clear();
            // filter_vector_z.clear();
            // calib_counter=0;


            // if (show_vector_x.size()==10){
            //             k=0;
            //             for (float i: show_vector_x){
            //                         k++;
            //                         if(k==10){std::cout<<i<<std::endl; k=0;}
            //                         std::cout<<i<<", ";
                        
                        
            //             }
            //             std::cout<<"----------------"<<std::endl;

            //             for (float i: show_vector_y){
            //                         k++;
            //                         if(k==10){std::cout<<i<<std::endl; k=0;}
            //                         std::cout<<i<<", ";

            //             }
            //             std::cout<<"----------------"<<std::endl;

            //             for (float i: show_vector_z){
            //                         k++;
            //                         if(k==10){std::cout<<i<<std::endl; k=0;}
            //                         std::cout<<i<<", ";
            //             }
            //             std::cout<<"----------------"<<std::endl;
            
                        
            // }


            k=0;
            for (float i: show_vector_x){
                        k++;
                        sums+=i;

                        if(k==100){
                
                                    std::cout<<"dx:"<<sums<<std::endl;
                                    std::cout<<"----------------"<<std::endl; 
                                    k=0;
                                    }
                        
            
            
            }

            


            sums=0;

          for (float i: show_vector_y){
                        k++;
                        sums+=i;

                        if(k==100){
                
                                    std::cout<<"dy:"<<sums<<std::endl; 
                                    std::cout<<"----------------"<<std::endl;
                                    k=0;
                                    }
                        
            
            
            }

            sums=0;

            for (float i: show_vector_z){
                        k++;
                        sums+=i;

                        if(k==100){
                
                                    std::cout<<"dz:"<<sums<<std::endl; 
                                    std::cout<<"----------------"<<std::endl;
                                    k=0;
                                    }
                        
            
            
            }
            show_vector_x.clear();
            show_vector_y.clear();
            show_vector_z.clear();



    std::cout<<" COLLECTED"<<std::endl;


//     if (calib_counter==40){

//             for (float i: filter_vector_x){
//                         sums += i;
                        
//             }
//             show_vector_x.emplace_back(sums/40);
//             sums=0;


//             for (float i: filter_vector_y){
//                         sums += i;
//             }
//             show_vector_y.emplace_back(sums/40);
//             sums=0;


//             for (float i: filter_vector_z){
//                         sums += i;
//             }
//             show_vector_z.emplace_back(sums/40);
//             sums=0;



//             filter_vector_x.clear();
//             filter_vector_y.clear();
//             filter_vector_z.clear();
//             calib_counter=0;


//             if (show_vector_x.size()==10){
//                         k=0;
//                         for (float i: show_vector_x){
//                                     k++;
//                                     if(k==10){std::cout<<i<<std::endl; k=0;}
//                                     std::cout<<i<<", ";
                        
                        
//                         }
//                         std::cout<<"----------------"<<std::endl;

//                         for (float i: show_vector_y){
//                                     k++;
//                                     if(k==10){std::cout<<i<<std::endl; k=0;}
//                                     std::cout<<i<<", ";

//                         }
//                         std::cout<<"----------------"<<std::endl;

//                         for (float i: show_vector_z){
//                                     k++;
//                                     if(k==10){std::cout<<i<<std::endl; k=0;}
//                                     std::cout<<i<<", ";
//                         }
//                         std::cout<<"----------------"<<std::endl;
            
                        
//             }





    }




    
//     if (calib_counter==48)
//     {
//         prev_time=ros::Time::now().toSec()+ros::Time::now().toNSec()/pow(10,9);
        
//         calib_counter++;
//         for (int i=0; i<48; i++){
        
//         sum_x += filter_vector_x[i];
//         sum_y += filter_vector_y[i];
//         sum_z += filter_vector_z[i];
        
//         }
//         calib_ax = sum_x/50;
//         calib_ay = sum_y/50;
//         calib_az = sum_z/50;
//         std::cout<<" DONE"<<std::endl;
        
//     }
    
    
//     //tim=(imu_msg->header.stamp.sec)+(imu_msg->header.stamp.nsec)/pow(10,9); //время = секунды + наносекунды

//     tim=ros::Time::now().toSec()+ros::Time::now().toNSec()/pow(10,9);
//     dt=(tim-prev_time);//*pow(10,-6);
    
//     std::cout<<tim<<" time"<<std::endl;
//     std::cout<<prev_time<<" prev_time"<<std::endl;
    
//     std::cout<<dt<<" dt"<<std::endl;
//     std::cout<<ros::Time::now()<<" rosTime"<<std::endl;
    
    
//     ax=imu_msg->linear_acceleration.x-calib_ax;      //принимаем ускорения
//     ay=imu_msg->linear_acceleration.y-calib_ay;
    
//     //vx=ax*dt;
//     //vy=ay*dt;
    
//     cur_x+=pax*dt+(ax*pow(dt,2))/2;//X=X0+Vx*dt+a*(dt^2)/2
//     cur_y+=pay*dt+(ay*pow(dt,2))/2;
    
//     prev_time=tim;                          //обновляем прошлое время
//     prev_x=cur_x;
//     prev_y=cur_y;
//     pax=ax;
//     pay=ay;
    
//     pvx=pax*dt;                             //обновляем прошлоую скорость
//     pvy=pay*dt;
    
//     //p2vx+=pax*dt;
//     //p2vx+=pax*dt;
    
//     std::cout<<"x: "<<cur_x<<std::endl;
//     std::cout<<"y: "<<cur_y<<std::endl;
//     std::cout<<"ax: "<<ax<<std::endl;
//     std::cout<<"ay: "<<ay<<std::endl;
//     std::cout<<"--------"<<std::endl;



//     std::cout<<"ax: "<<ax<<std::endl;
//     std::cout<<"ay: "<<ay<<std::endl;
//     std::cout<<"--------"<<std::endl;
    
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
    
    ros::Rate r(1.0);
    while(s.ok()){

        ros::spinOnce();               // check for incoming messages

//         if (theta>1.5708){
//             std::cout<<"right"<<std::endl;
//             velocity.linear.x=0.05;
//             velocity.angular.z=abs(1.5708-theta)*0.2;
//             vel_pub.publish(velocity);
//         }

     //   velocity.linear.x=0.05;
     //   velocity.angular.z=(-theta)*K;
     //   vel_pub.publish(velocity);
        
        ros::Duration(0.1).sleep();
        //r.sleep();
    }
}