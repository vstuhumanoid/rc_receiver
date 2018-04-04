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
    publisher = node.advertise<rc_receiver::PultData>("pult", 1000);
    for(int i = 0; i < 14; i++)
        message.channels.push_back(0);
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
    QString pultData;
    for(int i = 0; i < packet.size(); i++)
        pultData += QString::number(packet[i]) + " ";

    ui->pultDataEdit->append("\n" + pultData);

    publishMessage(packet);

}

void MainWindow::publishMessage(QList<short> packet)
{
    for(int i = 0; i < packet.size(); i++)
        message.channels[i] = packet[i] - 1000;

    publisher.publish(message);
}
