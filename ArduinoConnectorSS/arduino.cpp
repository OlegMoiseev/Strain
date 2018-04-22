#include <iostream>

#include <QSerialPort>

#include "arduino.h"


QString Arduino::identPort()
{
    std::cout << "Availible COM ports\n\n";

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        std::cout << "Name: " << info.portName().toStdString() << '\n';
        std::cout << "Description: " << info.description().toStdString() << '\n';
        std::cout << "Manufacturer: " << info.manufacturer().toStdString() << "\n\n";
    }

    std::cout << "Enter number of the COM port: ";
    std::string tmpStr;
    std::getline(std::cin, tmpStr);
    tmpStr = "COM" + tmpStr;

    return QString(tmpStr.c_str());
}

uint32_t Arduino::dataToUint32(QByteArray data)
{
    return ((uint32_t)(uint8_t)data[0]) + (((uint32_t)(uint8_t)data[1]) << 8)
    + (((uint32_t)(uint8_t)data[2]) << 16) + (((uint32_t)(uint8_t)data[3]) << 24);
}

Arduino::Arduino(const QString &portName)
    : _serial(new QSerialPort(portName)),
      _dataReady(false),
      _requestForPeriod(new char[5] { 8, 0, 0, 0, 0 }),
      _requestForData(new char[5] { 1, 0, 0, 0, 0 })
{
    _serial->open(QSerialPort::ReadWrite);
    _serial->setBaudRate(QSerialPort::Baud9600);
    _serial->setDataBits(QSerialPort::Data8);
    _serial->setParity(QSerialPort::NoParity);
    _serial->setStopBits(QSerialPort::OneStop);
    _serial->setFlowControl(QSerialPort::NoFlowControl);

    if (_serial->isOpen() && _serial->isWritable() && _serial->isReadable())
    {
            std::cout << "Opened successfully!\n";
    }
    else
    {
        std::cout << "Port wasn't opened!\n";
    }


    //connect(this, &Arduino::getPeriod, this, &Arduino::readPeriod);
    //connect(this, &Arduino::getData, this, &Arduino::readData);
    connect(_serial, &QSerialPort::readyRead, this, &Arduino::read);
}

void Arduino::read()
{
    std::cout << "Called function \"read\"\n";
    _answerCode = _serial->read(1);
    int code = _answerCode[0] + '0' - 48;
    std::cout << "Answer: " << code << '\n';

    switch (code)
    {
    case 5:
        _period = Arduino::dataToUint32(_serial->read(4));
        std::cout << "Period = " << _period << '\n';
        break;

    default:
        for (size_t i = 0; i < 6; ++i)
        {
            _data.at(i) = _serial->read(2);
            std::cout << _data.at(i).toInt() << '\t';
        }
        std::cout << '\n';
        break;
    }
    _dataReady = true;
}

void Arduino::writeData(const QByteArray &data) const
{
    if (_serial->isOpen() && _serial->isWritable())
    {
        std::cout << "Bytes send: " << _serial->write(data) << '\n';
    }
    else
    {
        std::cout << "Port wasn't opened!\n";
    }
}

uint32_t Arduino::getPeriod()
{
    std::cout << "Called function \"getPeriod\"\n";
    _dataReady = false;
    writeData(QByteArray(_requestForPeriod, 5));
    while(!_dataReady)
    {
        continue;
    }
    return _period;
}

std::array<int, 6> Arduino::getData()
{
    std::cout << "Called function \"getData\"\n";
    _dataReady = false;
    writeData(QByteArray(_requestForData, 5));
    while(!_dataReady)
    {
        continue;
    }

    std::array<int, 6> tmp;
    for (size_t i = 0; i < 6; ++i)
    {
        tmp.at(i) = _data.at(i).toInt();
    }
    return tmp;
}
