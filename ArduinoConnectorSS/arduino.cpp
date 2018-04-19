#include <iostream>

#include <QSerialPort>

#include "arduino.h"


Arduino::Arduino(const QString &portName)
    : _serial(new QSerialPort(portName)),
      _requestForPeriod(new char[5] { 8, 0, 0, 0, 0 }),
      _requestForData(new char[5] { 1, 0, 0, 0, 0 })
{
    connect(_serial, &QSerialPort::readyRead, this, &Arduino::read);
    connect(this, &Arduino::getPeriod, this, &Arduino::readPeriod);
    connect(this, &Arduino::getData, this, &Arduino::readData);

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
}

QString Arduino::identPort()
{
    QTextStream cin(stdin);

    std::cout << "Availible COM ports\n\n";

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        std::cout << "Name: " << info.portName().toStdString() << '\n';
        std::cout << "Description: " << info.description().toStdString() << '\n';
        std::cout << "Manufacturer: " << info.manufacturer().toStdString() << "\n\n";
    }

    std::cout << "Enter number of the COM port: ";
    QString tmpStr = "COM" + cin.readLine();

    return tmpStr;
}


void Arduino::read()
{
    std::cout << "Function \"read\" called\n";
    _answerCode = _serial->read(1);
    _periodRaw = _serial->read(4);
    _period = _periodRaw.toUInt();

    std::cout << "Answer: " << _answerCode[0] + '0' - 48;
    std::cout << "Period = " << _period;
}

void Arduino::writeData(const QByteArray data) const
{
    std::cout << "Function \"writeData\" called\n";
    if (_serial->isOpen() && _serial->isWritable())
    {
        std::cout << "Bytes send: " << _serial->write(data) << '\n';
        //_serial->waitForBytesWritten(1000);
    }
    else
    {
        std::cout << "Port wasn't opened!\n";
    }
}

void Arduino::readPeriod() const
{
    std::cout << "Function \"readPeriod\" called\n";
    writeData(QByteArray(_requestForPeriod, 5));
    // Теперь здесь мне надо вызвать функцию, которая распарсит данные, которые пришли в порт.
    // Как это сделать? Я же не знаю в данном моменте, считались ли уже данные или нет,
    // так как за чтение отвечает функция
}

void Arduino::readData() const
{
    writeData(QByteArray(_requestForPeriod, 5));
    // Та же проблема. Здесь должна будет вызвана уже другая функция, в отличие от предыдущего случая.
}
