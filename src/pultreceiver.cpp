#include "pultreceiver.h"

PultReceiver::PultReceiver(QObject *parent) : QObject(parent)
{
    initPort();

    packageBuilder = new PackageBuilder(QByteArray(header), packageLength);
    pultChannels.clear();

    connect(&port, SIGNAL(readyRead()), this, SLOT(onReceivedData()));
    connect(packageBuilder, SIGNAL(packageReceived(QByteArray)), this, SLOT(onReceivedPackage(QByteArray)));
}

PultReceiver::~PultReceiver()
{
}

QStringList PultReceiver::GetComPortsList()
{
    QSerialPortInfo info;
    QList<QSerialPortInfo> ports = info.availablePorts();
    QStringList list;
    for(int i = 0; i < ports.size(); i++)
        list.append(ports[i].portName());
    return list;
}

void PultReceiver::initPort()
{
    port.setBaudRate(QSerialPort::Baud115200);
    port.setDataBits(QSerialPort::Data8);
    port.setStopBits(QSerialPort::OneStop);
    port.setParity(QSerialPort::NoParity);
}

void PultReceiver::onReceivedData()
{
    QByteArray data;
    data.append(port.readAll());

    packageBuilder->processPart(data);

    dataReceived(data);
}

void PultReceiver::onReceivedPackage(QByteArray package)
{
    int channels = (package.size() - 2) / 2;
    pultChannels.clear();

    for(int i = 0; i < channels; i++)
    {
        short value = *((short*)(package.data() + i*2));
        pultChannels.append(value);
    }

    packageReceived(pultChannels);
}

bool PultReceiver::OpenPort(QString portName)
{
    if(port.isOpen())
        ClosePort();
    port.setPortName(portName);
    initPort();
    return port.open(QIODevice::ReadOnly);
}

void PultReceiver::ClosePort()
{
    port.close();
    portClosed();
}

