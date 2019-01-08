#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "ros/ros.h"

double x,y,z,k,p,q,vel_r,vel_l,vel_car;
float d,r;
//const float d=170; //distance between wheel in cm
const float r=20;  //radius of wheel in cm
const float k=8.6; //k=(distance between wheel/radius of wheel)

void gr_raw(const std_msgs::Float64::ConstPtr& msg)
{
   p=msg->data;
   //ROS_INFO("I heard: [%f]", msg->data);
}
void gl_raw(const std_msgs::Float64::ConstPtr& msg)
{
   q=msg->data;
}

void calVel()
{
	// x=OmegaR y=OmegaL z=OmegaC 
	// k=(distance between wheel)/(radius of wheel) p=gR q=gL 
	// Equations solve(x=y+z*k,p*p=x*x+z*z,q*q=y*y+z*z)
	double k_sq=k*k;
	double p_sq=p*p;
	double q_sq=q*q;
	double exp1=(k_sq*p_sq+p_sq-q_sq);
	if(k*(k_sq + 4)!=0 && exp2!=0)
	{
		double exp2=sqrt((k_sq*k_sq*p_sq+3*k_sq*p_sq-2*sqrt(k_sq*(k_sq*p_sq*q_sq-p_sq*p_sq+2*p_sq*q_sq-q_sq*q_sq))-k_sq*q_sq)/(k_sq*(k_sq+4)));
		double exp3=sqrt(k_sq*(k_sq*p_sq*q_sq-p_sq*p_sq+2*p_sq*q_sq-q_sq*q_sq));

		x= exp2; //angural velocity of wheel (R) in RPM
		y= ((p*p - q*q - exp3)*exp2)/exp1; 
				//angural velocity of wheel (L) in RPM
		z= (k*k*p*p+exp3)*exp2/(exp1*k); //angural velocity of car in RPM
	}

	vel_r=x*r;
	vel_l=y*r;
	vel_car=((x+y)*r)/2;
}

int main(int argc, char**argv)
{
	ros::init(argc, argv, "imu_velocity_calc");
	ros::NodeHandle nh;
	geometry_msgs::Twist msg;	
	ros::Subscriber sub_r = nh.subscribe("/imu_data/gr_raw",1000,&gr_raw);
	ros::Subscriber sub_l = nh.subscribe("/imu_data/gl_raw",1000,&gl_raw);
	ros::Publisher pub =nh.advertise<geometry_msgs::Twist>("/imu_enc/vel", 1000);

	//ros::Rate rate(2);
	while(ros::ok()){
		calVel();
		msg.linear.x =	vel_r;		//Linear Right Wheel
		msg.linear.y = 	vel_l;		//Linear Left Wheel
		msg.linear.z =	vel_car;	//Linear Car Velocity
		msg.angular.x =	x;  		//Omega of Right Wheel
		msg.angular.y =	y;  		//Omega of Left Wheel
		msg.angular.z =	z;  		//Omega of Car

		pub.publish(msg);

		//ROS_INFO_STREAM("Angular velocity: "<<" OmegaR = "<<msg.angular.x<<" OmegaL = "<<msg.angular.y" OmegaC = "<<msg.angular.z);

		//rate.sleep();

		}
}
