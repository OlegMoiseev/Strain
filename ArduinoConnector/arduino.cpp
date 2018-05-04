#include <iostream>

#include <QSerialPortInfo>

#include "arduino.h"


Arduino::Arduino(const QString &portName):
    _requestForPeriod{ 8, 0, 0, 0, 0 },
    _requestForData{ 1, 0, 0, 0, 0 }
{
    _serial = new QSerialPort(portName);
    _serial->open(QSerialPort::ReadWrite);
    _serial->setBaudRate(QSerialPort::Baud9600);
    _serial->setDataBits(QSerialPort::Data8);
    _serial->setParity(QSerialPort::NoParity);
    _serial->setStopBits(QSerialPort::OneStop);
    _serial->setFlowControl(QSerialPort::NoFlowControl);

    if (_serial->isOpen() && _serial->isWritable() && _serial->isReadable())
    {
        qDebug() << "Opened successfully!\n";
    }
}


void Arduino::writeData(const QByteArray data) const
{
    if (_serial->isOpen() && _serial->isWritable())
    {
        qDebug() << "Bytes send: " << _serial->write(data) << '\n';
    }
    else
    {
        qDebug() << "Port wasn't opened!\n";
    }
}


QString Arduino::identPort()
{
    qDebug() << "Availible COM ports\n\n";

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name: " << info.portName()
                 << "\nDescription: " << info.description()
                 << "\nManufacturer: " << info.manufacturer() << "\n\n";
    }

    qDebug() << "Enter number of the COM port:";
    std::string tmpStr;
    std::getline(std::cin, tmpStr);
    tmpStr = "COM" + tmpStr;

    return QString(tmpStr.c_str());
}


uint32_t Arduino::getPeriod()
{
    //writeData(QByteArray(_requestForPeriod, bytesInRequest));
    QByteArray req = QByteArrayLiteral("\010\000\000\000\000");
    writeData(req);

    _serial->waitForReadyRead(waitingTime);

    int availBytes = _serial->bytesAvailable();
    qDebug() << "Bytes available to read: " << availBytes;

    char actualData[4];
    uint32_t period;

    if (availBytes == needBytesFromSensor)
    {
        _serial->read(answerCode, availBytes - bytesData);  // read code of answer from the Arduino
        _serial->read(actualData, availBytes - bytesCode);  // read period of work from the Arduino
        memcpy(&period, actualData, bytesData);

        qDebug() << "Answer: " << answerCode[0] + '0' - 48;
        qDebug() << "Period = " << period;
        return period;
    }
    else
    {
        if (availBytes == 0)
        {
            qDebug() << "No bytes available";
            std::cin.get();
            return -1;
        }
        else
        {
            qDebug() << "Bytes available: " << availBytes << "\nIt's wrong!";
            std::cin.get();
            return -2;
        }
    }
    _serial->close();
    std::cin.get();
}

std::array<int, 6> Arduino::getData()
{
    char dataFromSensor[2];
    int16_t tmpBuffer;
    std::array<int, 6> data;

    writeData(QByteArray(_requestForData, bytesInRequest));

    _serial->waitForReadyRead(waitingTime);

    int availBytes = _serial->bytesAvailable();

    if (availBytes == 13)
    {
        _serial->read(answerCode, bytesCode);  // read code of answer from the Arduino

        for (size_t i = 0; i < _numOfChannels; ++i)
        {
            _serial->read(dataFromSensor, _bytesInOneAnswer);  // read data from the Arduino & sensor
            memcpy(&tmpBuffer, dataFromSensor, _bytesInOneAnswer);
            data.at(i) = tmpBuffer;
        }
    }
    else
    {
        if (availBytes == 0)
        {
            qDebug() << "No bytes available\n";
        }
        else
        {
            qDebug() << "Bytes available: " << availBytes << "\nIt's wrong!";
            std::cin.get();
        }
        std::cin.get();
    }

    for (const auto elem : data)
    {
        qDebug() << elem << ' ';
    }
    qDebug() << '\n';
    return data;
}
