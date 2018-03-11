#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QtCore>
#include <iostream>


int main()
{

    QCoreApplication a();

    std::cout << "Availible COM ports\n\n";

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        std::cout << "Name: " << info.portName().toStdString() << '\n';
        std::cout << "Description: " << info.description().toStdString() << '\n';
        std::cout << "Manufacturer: " << info.manufacturer().toStdString() << "\n\n";
    }

    std::cout << "Enter number of the COM port: ";
    std::string tmpStr;
    std::getline(std::cin, tmpStr);
    tmpStr = "COM" + tmpStr;
    const QString portName(tmpStr.c_str());

    QSerialPort serial;
    serial.setPortName(portName);
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::HardwareControl);
    serial.open(QIODevice::ReadWrite);
   

    char symToSend[1];
    while (true)
    {
        std::cin >> symToSend;
        serial.write(symToSend);

        serial.waitForBytesWritten(10);
        while (serial.bytesAvailable() > 0 || serial.waitForReadyRead(10))
        {
            QByteArray ba = serial.readLine();
           
            std::cout << ba.toStdString() << '\n';
        }
    }
    serial.close();
}