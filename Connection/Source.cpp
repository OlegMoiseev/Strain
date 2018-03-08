#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <iostream>

int main()
{
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
    serial.open(QIODevice::WriteOnly);

    if (serial.isOpen() && serial.isWritable())
    {
        std::cout << "Serial port is opened\n";

        char buf[1];

        const auto bytesWritten = serial.write("1", 1);
        serial.waitForBytesWritten(10);
        std::cout << bytesWritten << '\n';
        //serial.waitForReadyRead(10000);

        /*while (true)
        {
            serial.waitForReadyRead(1);
            serial.read(buf, 1);
            std::cout << buf[0];

        }*/
    }
    std::cin.get();
    return 0;
}