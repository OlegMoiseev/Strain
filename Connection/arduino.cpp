#include <iostream>

#include <QtSerialPort/QSerialPortInfo>

#include "arduino.h"


Arduino::Arduino(const QString &portName)
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
        std::cout << "Opened successfully!\n";
    }
}


void Arduino::writeData(const QByteArray data) const
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


void Arduino::readData(char* buf, const int bytes) const
{
   std::cout << _serial->read(buf, bytes);
}
