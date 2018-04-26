#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_refreshComsButton_clicked();
    connect(&pult, SIGNAL(portClosed()), this, SLOT(pultPortClosed()));
    //connect(&pult, SIGNAL(dataReceived(QByteArray)), this, SLOT(pultDataReceived(QByteArray)));
    connect(&pult, SIGNAL(packageReceived(QList<short>)), this, SLOT(pultPacketReceived(QList<short>)));

    initRos();
}

MainWindow::~MainWindow()
{
    delete(ui);
}

void MainWindow::initRos()
{
    data_publisher_ = nh_.advertise<rc_receiver::PultData>("rc_receiver/pult_data", 1000);
    for(int i = 0; i < 14; i++)
        message.channels.push_back(0);

    params_publisher_ = nh_.advertise<robot_controllers::MotionParams>("/frund/motion_params", 1000);
}

void MainWindow::on_refreshComsButton_clicked()
{
    ui->comsComboBox->clear();
    ui->comsComboBox->addItems(pult.GetComPortsList());
}

void MainWindow::on_connectPultButton_clicked()
{
    if(pult.OpenPort(ui->comsComboBox->currentText()))
        ui->pultStatusLabel->setText("Пульт подключен");
    else
        ui->pultStatusLabel->setText("Пульт не подключен");
}

void MainWindow::on_disconnectPultButton_clicked()
{
    pult.ClosePort();
}

void MainWindow::pultPortClosed()
{
    ui->pultStatusLabel->setText("Пульт не подключен");
}

void MainWindow::pultDataReceived(QByteArray data)
{
    ui->pultDataEdit->append(data + " ");
}

void MainWindow::pultPacketReceived(QList<short> packet)
{
	if(++packets_count_ == 100)
	{
	    QString pultData;
	    for(int i = 0; i < packet.size(); i++)
	        pultData += QString::number(packet[i]) + " ";

	    ui->pultDataEdit->setText(pultData);

	    publishMessage(packet);

	    packets_count_ = 0;
	}

}

void MainWindow::publishMessage(QList<short> packet)
{
	for(int i = 0; i < packet.size(); i++)
    	message.channels[i] = packet[i] - 1500;

    data_publisher_.publish(message);

    //TODO: сделать правильное задание параметров (масштабирующие коэффициенты)
    motionParams_.velocity_x = (double)(packet[3] - 1500) / 500.0 * 0.03;
    motionParams_.velocity_y = (double)(packet[2] - 1500) / 500.0 * 0.03;
    motionParams_.angular_velocity = (double)(packet[1] - 1500) / 500.0 * 0.02;
    motionParams_.com_height = (double)(packet[0] - 1500) / 500.0 * 0.1 + 0.5;
    motionParams_.step_height = (double)(packet[4] - 1500) / 500.0 * 0.1 + 0.1;
    motionParams_.step_length = (double)(packet[5] - 1500) / 500.0 * 0.2 + 0.3;
    params_publisher_.publish(motionParams_);
}
