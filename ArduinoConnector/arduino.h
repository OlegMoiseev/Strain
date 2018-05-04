#ifndef ARDUINO_H
#define ARDUINO_H

#include <array>

#include <QSerialPort>
#include <QtCore>

class Arduino
{
private:
    QByteArray _recievedData;

    const int waitingTime = 1000;
    const int needBytesFromSensor = 5;
    const int bytesInRequest = 5;

    const int bytesCode = 1;
    const int bytesData = 4;

    char answerCode[1];

    char _requestForPeriod[5];
    char _requestForData[5];


    const int _numOfChannels = 6;
    const int _bytesInOneAnswer = 2;

public:
    QSerialPort* _serial;

    explicit Arduino(const QString &portName);

    void writeData(QByteArray data) const;

    static QString identPort();

    uint32_t getPeriod();

    std::array<int, 6> getData();
};
#endif // ARDUINO_H
