#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>

ros::Publisher pub;
geometry_msgs::Twist vel;

void callback(const std_msgs::Float32ConstPtr& dist) {
   if (dist->data != 0 ) {
         vel.linear.x = -10;
         vel.angular.z =10;
     } else{ 
         vel.linear.x = 10;
         vel.angular.z = 0;
     }
    pub.publish(vel);
}

int main(int argc, char **argv){
    ros::init(argc, argv, "sonarController");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/vrep/vehicle/frontSonar", 1, callback);
    pub = n.advertise<geometry_msgs::Twist>("/sonarController", 1);
    
    ros::spin();
    
}