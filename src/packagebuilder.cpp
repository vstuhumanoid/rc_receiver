#include "packagebuilder.h"

PackageBuilder::PackageBuilder(QObject *parent) : QObject(parent)
{

}

PackageBuilder::PackageBuilder(QByteArray header, int packageLength)
{
    _packageHeader.clear();
    _packageHeader.append(header);

    _packageLength = packageLength - header.size();

    _receivedQueue.clear();
    _currentHeaderByte = 0;
    _state = RECEIVING_HEADER;
}

void PackageBuilder::processPart(QByteArray buffer)
{
    for(int i = 0; i < buffer.size(); i++)
    {
        if (_state == RECEIVING_HEADER)
        {
            //Ждем заголовка

            if (buffer[i] == _packageHeader[_currentHeaderByte])
            {
                //Читаем заголовок
                _state = RECEIVING_HEADER;
                _currentHeaderByte++;
            }
            else
            {
                //Это не заголовок
                _currentHeaderByte = 0;
                _state = RECEIVING_HEADER;
            }

            if (_currentHeaderByte == _packageHeader.size())
            {
                //Заголовок полностью считан
                _state = RECEIVING_BODY;
            }
        }
        else if (_state == RECEIVING_BODY)
        {
            //Читаем пакет
            _receivedQueue.append(buffer[i]);

            if (_receivedQueue.size() == _packageLength)
            {
                //Пакет полностью считан
                _state = RECEIVING_HEADER;
                _currentHeaderByte = 0;

                packageReceived(_receivedQueue);

                _receivedQueue.clear();
            }
        }
    }
}