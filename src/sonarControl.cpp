#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>

ros::Publisher pub;
geometry_msgs::Twist vel;
float teste_left,teste_right; //usado para levar informações entre os sensores 
int contador = 0;                            // se o sensor estiver bloqueado teste = 1

void cb_front(const std_msgs::Float32ConstPtr& dist) {
    if (contador == 0){ 
        if (dist->data > 0.2) {
            vel.linear.x = - (1 - dist->data)*(1 - dist->data) * 20 ;
        }else if(dist->data != 0 && dist->data < 0.2){
             contador=1;
        }
         else {
            vel.linear.x = 0 ;
        }
        if ( (teste_left < 0.5) && (teste_right < 0.5) && teste_right != 0 && teste_left != 0){
            vel.angular.z = ( (1- teste_left) - ( 1- teste_right) ) * 3;
        }else if ( (teste_right == 0) && (teste_left < 0.5) && teste_left != 0) {
            vel.angular.z = -5;
        } else if ( ( teste_right < 0.5) && (teste_left == 0 )&& teste_right != 0 ){
            vel.angular.z = 5;
        } else if (dist->data < 0.8 && teste_left < 0.8 && teste_left != 0 && ( dist->data !=0) ){
            vel.angular.z = -15;
        } else if ( dist->data < 0.8 && teste_right < 0.8 && teste_right != 0 && dist->data !=0 ){
            vel.angular.z = 15;
        }
        else vel.angular.z = 0;
        } 
    else {
        vel.linear.x = -30;
        vel.angular.z = -30;   
    }
    if( dist->data > 0.5 || dist->data == 0){
        contador = 0;
    }
    
    std:: cout << "counter: " << contador << std::endl;
    
    pub.publish(vel);
    
}

void cb_left(const std_msgs::Float32ConstPtr &dist )
{    if (dist->data !=0) {
        teste_left= dist->data; 
    } else teste_left = 0;
}

void cb_right(const std_msgs::Float32ConstPtr &dist) // se o sensor do carro apitar, o carro vira
{  if (dist->data !=0) {
        teste_right = dist->data ; 
    } else teste_right = 0;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "carrot_sonarController");
    ros::NodeHandle n;
    teste_left=0;
    teste_right=0;
    ros::Subscriber sub_front = n.subscribe("/vrep/carrot/frontSonar", 1, cb_front); 
    ros::Subscriber sub_left = n.subscribe("/vrep/carrot/leftSonar", 1, cb_left); 
    ros::Subscriber sub_right = n.subscribe("/vrep/carrot/rightSonar", 1, cb_right); 
    pub = n.advertise<geometry_msgs::Twist>("/sonar", 1); 
    ros::spin();
    
}