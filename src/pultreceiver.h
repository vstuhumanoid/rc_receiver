#ifndef RC_RECEIVER_PULTRECEIVER_H
#define RC_RECEIVER_PULTRECEIVER_H

#include <QObject>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QString>
#include "packagebuilder.h"

class PultReceiver : public QObject
{
Q_OBJECT

public:
    explicit PultReceiver(QObject *parent = 0);
    ~PultReceiver();
    QStringList GetComPortsList();
    bool OpenPort(QString portName);
    void ClosePort();

private:
    QSerialPort port;
    PackageBuilder *packageBuilder;
    char header[2] = { 0x20, 0x40 };
    int packageLength = 32;
    QList<short> pultChannels;

    void initPort();

private slots:
    void onReceivedData();
    void onReceivedPackage(QByteArray package);

signals:
    void dataReceived(QByteArray data);
    void packageReceived(QList<short> pultData);
    void portClosed();
};

#endif //RC_RECEIVER_PULTRECEIVER_H
