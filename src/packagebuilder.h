#ifndef RC_RECEIVER_PACKAGEBUILDER_H
#define RC_RECEIVER_PACKAGEBUILDER_H

#include <QObject>

class PackageBuilder : public QObject
{
    Q_OBJECT

    enum ReceiveState
    {
        RECEIVING_HEADER,
        RECEIVING_BODY
    };

public:
    explicit PackageBuilder(QObject *parent = 0);
    PackageBuilder(QByteArray header, int packageLength);

    void processPart(QByteArray buffer);

signals:
    void packageReceived(QByteArray package);

private:
    ReceiveState _state;

    int _currentHeaderByte;
    int _packageLength;
    QByteArray _packageHeader;
    QByteArray _receivedQueue;
};

#endif //RC_RECEIVER_PACKAGEBUILDER_H
