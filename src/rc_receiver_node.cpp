#include <ros/ros.h>
#include <std_msgs/String.h>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "rc_receiver_node");
    ROS_INFO("rc_receiver_node started");

    /*ros::NodeHandle node;

    ros::Rate rate(1);

    while(ros::ok())
    {
        ROS_INFO("Hello from rc_receiver");

        ros::spinOnce();

        rate.sleep();
    }*/

    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}
