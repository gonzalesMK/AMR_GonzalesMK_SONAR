#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>

ros::Publisher pub;
geometry_msgs::Twist vel;
int teste_left,teste_right; //usado para levar informações entre os sensores 
                            // se o sensor estiver bloqueado teste = 1

void cb_front(const std_msgs::Float32ConstPtr& dist) {
   if (dist->data != 0) { // se o sensor marcar alguma coisa, o carro vai devagar
         vel.linear.x = 0.5;
  } else vel.linear.x = 3;        // senão, ele corre
    
    if (!( teste_left || teste_right) ) vel.angular.z = 0; //se nenhum dos sensores laterais apitar, o carro vai reto
    pub.publish(vel);
}

void cb_left(const std_msgs::Float32ConstPtr &dist )
{
    if ( dist->data != 0){         // se o sensor da direita apitar, o carro vira
        vel.angular.z = -3 ;   
        pub.publish(vel);
        teste_left = 1;         // se o sensor apitar 
    }  else teste_left = 0 ;
}

void cb_right(const std_msgs::Float32ConstPtr &dist ) // se o sensor do carro apitar, o carro vira
{
    if ( dist->data != 0){
        vel.angular.z = 3 ;
        pub.publish(vel);
        teste_right=1;
    } else teste_right =0;
    
}

int main(int argc, char **argv){
    ros::init(argc, argv, "sonarController");
    ros::NodeHandle n;
    teste_left=0;
    teste_right=0;
    ros::Subscriber sub_front = n.subscribe("/vrep/vehicle/frontSonar", 1, cb_front); //topico para o sensor da frente
    ros::Subscriber sub_left = n.subscribe("/vrep/vehicle/leftSonar", 1, cb_left); //topico left
    ros::Subscriber sub_right = n.subscribe("/vrep/vehicle/rightSonar", 1, cb_right); //topico para o sensor right
    pub = n.advertise<geometry_msgs::Twist>("/sonarController", 1); //topico para comunicar com Kinect
    
    ros::spin();
    
}