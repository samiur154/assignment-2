#include"ros/ros.h"
#include"geometry_msgs/Twist.h" // rostopic info /cmd_vel
#include"nav_msgs/Odometry.h"   // rostopic info /odom & base_pose_ground_truth 
#include"sensor_msgs/LaserScan.h" // rostopic info /base_scan
#include"std_msgs/Float32.h"	//message
#include"std_srvs/Empty.h"	//servies(for reset)

ros ::Publisher pub;

bool reset = false; // boolean variable to reset system
float f_th = 1.5; //Threshold for front side
float rl_th= 0.7;// Threshold for left and right side
float dr = 0.0; //Right side (only define variable)
float dl = 0.0; //Left side  (only define variable)
float df = 2.0; //front side
float lin_vel = 2.0; //linear velocity
int DEG = 360; //total angle

// change angle form radiant to degree by facility
#define RAD2DEG(x) ((x)*180./M_PI)	//M_PI, the ratio of a circle’s circumference to its diameter


//callback system when reset it
void commandCallback(const std_msgs::Float32::ConstPtr& msg)
{
	// if the command value is encoded as number three, set reset as true
	if(msg->data == 3.0) reset = true; 
	//otherwise consider the command value as robot's linear velocity
	else lin_vel= msg->data;
}

//callback to read sensor data from rostopic info /base_scan
void baseScanCallback(const sensor_msgs::LaserScan::ConstPtr& msg) //otherwise you can replace msg with scan
{
	// scan front side info if robot at 90°
	df = msg->ranges[DEG];
	
	// scan right side info if 0°<robot<90°
	for(int i=0; i<DEG; i++)  dr= dr+msg->ranges[i];
	dr = dr/DEG;
	
	// scan left side info if 90°<robot<180°
	for(int j=0; j<DEG; j++) dl= dl+msg->ranges[j+DEG];
	dl = dl/DEG;
}

int main (int argc, char **argv)
{
	// Initialize the node, setup the NodeHandle for handling the communication with the ROS and system
	
	ros::init(argc, argv, "controller1");
	ros::NodeHandle nh;
	
	// Define the subscriber to robot's position
	// baseScancallback riceve every time the position of robot use rostopic info /base_scan
	ros::Subscriber sub = nh.subscribe("/base_scan", 1,baseScanCallback);
	
	//commandCallback riceve reset command
	ros::Subscriber sub2 = nh.subscribe("/command", 1,commandCallback);
	
	//Pubblish velocity of robot use rostopic info /cmd_vel
	pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",1); 
	
	// reset position of robot use std_srvs/Empty.h
	ros::ServiceClient client = nh.serviceClient<std_srvs::Empty>("/reset_positions"); 
	std_srvs::Empty reset_srv;
	
	//client exist
	client.waitForExistence();
	
	ros::Rate loop_rate(10);
	
	//endless loop for read and controll position of robot
	while(ros::ok())
	{	
	
		// print position of robot 1.front side 2.right side 3.left side and 4. linear velocity 5.reset case
		printf("front distance: %f\n",df);
		printf("right distance: %f\n",dr);
		printf("left distance: %lf\n",dl);
		printf("linear Velocity: %lf\n",lin_vel);
		printf("reset case: %d\n",reset);
		printf("\n");
		
		//reset position of robot
		if(reset == true)
		{
		//reset servies
		client.call(reset_srv);
		reset = false;
		}
		
		//controll linear velocity & angular velocity
		geometry_msgs::Twist my_vel;
		
		//if robot sees nothing in front
		if(df > f_th) 
		{
			my_vel.linear.x = lin_vel;
			my_vel.angular.z= 0.0;
			printf("go on!! \n");
		
			//controll right side
			if(dr < rl_th)
			{
				my_vel.linear.x = 0.0;
				my_vel.angular.z= -2.0;
				printf("a little right \n");
			}
			
			//controll left side
			if(dl < rl_th)
			{
				my_vel.linear.x = 0.0;
				my_vel.angular.z= 2.0;
				printf("a little left \n");
			}
		}
		
		//if robot sees somthings in front
		else
		{
			//controll left side
			if(dl > dr)
			{
				while(df < f_th & ros::ok())
				{
					if(reset == true)
					{
						client.call(reset_srv);
						reset = false;
					}
					
					my_vel.linear.x = 0.0;
					my_vel.angular.z= 2.0;
					printf("a little left \n");
					pub.publish(my_vel);
					
					ros::spinOnce();
					loop_rate.sleep();
				}
			}
			//controll right side
			else
			{
				while(df < f_th & ros::ok())
				{
					if(reset == true)
					{
						client.call(reset_srv);
						reset = false;
					}
					
					my_vel.linear.x = 0.0;
					my_vel.angular.z= -2.0;
					printf("a little right \n");
					pub.publish(my_vel);
					
					ros::spinOnce();
					loop_rate.sleep();
				}
			}
		}
		
		pub.publish(my_vel);
		
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
