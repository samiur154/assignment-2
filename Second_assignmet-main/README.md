# Second_assignment Research Track 1


Introduction
================================

Robot simulator developed in ros environment in C++ language, the task of this assignmet circulate robots in a Formula 1 track without touching the edges and reset the position and back to fisrt position and change the linear speed. All that is, you have to develop in this assignment, learn how to create a node in ros, how to use different packages in ros, use different servis in ros and know the type of servis or the format, how to implement a publisher and a subscriber in C++ language, how nodes communicate with each other, how to create a workspace in ros , how to use bashrc files and know the most important commands in ros.

Materials and Methods
=========================

This assignment was done by prof._Carmine Tommaso Recchiuto_ in course Reasearch track 1. All materials related to this course can be found on the website (https://2021.aulaweb.unige.it/course/view.php?id=4729) and to learn more about ros and learn from scratch check the website (http://wiki.ros.org/Documentation). The implemented method explained in the flowchat part and for startup part can refer to part Installing and running.

Installing and running
----------------------
After installing ros on the operating system(in my case operating system is a linux), you can proceed the following steps:
1. First step create your own workspace in my system :
```   
 $ mkdir –p/my_ros_ws/src
```
2. Enter new directory created :  
```  
 $ cd –p/my_ros_ws/src
```
3. Download in zip format or use fork for your repositories or use :
```  
4. Back in root file : 
```   
 $ cd ..
```
5. Use :
```     
 $ catkin_make (Catkin is the official build system of ROS and the successor to the original ROS build system, rosbuild)
```
6. Update setup.bash files to know new content :
```  
 $ source devel/setup.bash
```
7. Use :  
```
 $ roscore & (is the first thing you should run when using ROS).
```
8. Open the map where robots circulate :    
```
$ rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
```

Now then visualize this map in your system
At this point you have to open a new terminal always in root folder and update again setup. bash file and start the first node :
```
$ source devel/setup.bash   
$ rosrun second_assignment controller_node  
```
The first node practically circulates robots inside the circuit, without hitting against edges.  
Now you can start the reset node as it changes the speed of the robot and resets the position of the robot, but first you have to open a new terminal with the same addresses said for controller node and do the following commands :  
```
$ source devel/setup.bash
$ rosrun second_assignment reset_node
```
Now activated command_node which then change speed and reset robot position.  
In case if you are interested to know various information you can use the following commands:
```
$rostopic list  //Check all Topic
$rosservis list //Check all Servies
$rosnode list   //Check all Node
```
To run a program in C++ you have to modify the __CMakelists.txt__ and uncomment, line where  is written:
```
add_executable (name_node src/name_file.cpp)
```
Plus uncomment also, line where is written:
```
target_link_libraries (name_node
     ${catkin_LIBRARIES}
 )
 ```
If there are more than one C++ file to be executed together, as in this case (reset.cpp), you have to do the same procedure for the new files.  
Inside your workspace you can also find a file called __pakage.xlm__, this file is useful in case you need to add any package or some servies, for more information on this part you can refer to the site: (http://wiki.ros.org/catkin/package.xml)


