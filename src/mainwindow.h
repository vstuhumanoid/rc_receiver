#ifndef RC_RECEIVER_MAINWINDOW_H
#define RC_RECEIVER_MAINWINDOW_H

#include <QtWidgets>
#include <ui_mainwindow.h>
#include "pultreceiver.h"
#include "ros/ros.h"
#include <rc_receiver/PultData.h>
#include <robot_controllers/MotionParams.h>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PultReceiver pult;

    ros::NodeHandle nh_;
    ros::Publisher data_publisher_;
    rc_receiver::PultData message;
    ros::Publisher params_publisher_;
    robot_controllers::MotionParams motionParams_;

    int packets_count_ = 0;

    void initRos();
    void publishMessage(QList<short> packet);

private slots:
    void on_refreshComsButton_clicked();
    void on_connectPultButton_clicked();
    void on_disconnectPultButton_clicked();
    void pultPortClosed();
    void pultDataReceived(QByteArray data);
    void pultPacketReceived(QList<short> packet);
};

#endif //RC_RECEIVER_MAINWINDOW_H
