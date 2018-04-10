#include <iostream>
#include <array>

#include <QtCore/QCoreApplication>

#include "arduino.h"


int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    constexpr int waitingTime = 1000;
    constexpr int needBytesFromSensor = 5;
    constexpr int bytesInRequest = 5;

    constexpr int bytesCode = 1;
    constexpr int bytesData = 4;

    char answerCode[bytesCode];
    char actualData[bytesData];

    uint32_t period;

    constexpr char requestForPeriod[] = { 0x8, 0x0, 0x0, 0x0, 0x0 };

    Arduino connector(Arduino::identPort());

    connector.writeData(QByteArray(requestForPeriod, bytesInRequest));

    connector._serial->waitForReadyRead(waitingTime);
    int availBytes = connector._serial->bytesAvailable();
    std::cout << "Bytes available to read: " << availBytes << '\n';
    

    if (availBytes == needBytesFromSensor)
    {
        connector._serial->read(answerCode, availBytes - bytesData);  // read code of answer from the Arduino
        connector._serial->read(actualData, availBytes - bytesCode);  // read period of work from the Arduino
        memcpy(&period, actualData, bytesData);

        std::cout << "Answer: " << answerCode[0] + '0' - 48;
        std::cout << "Period = " << period;
    }
    else
    {
        if (availBytes == 0)
        {
            std::cout << "No bytes available\n";
        }
        else
        {
            std::cout << "Bytes available: " << availBytes << "\nIt's wrong!";
            std::cin.get();
            return 1;
        }
        std::cin.get();
        return 2;
    }

    constexpr char requestForData[] = { 0x1, 0x0, 0x0, 0x0, 0x0 };
    constexpr int numOfChannels = 6;
    constexpr int bytesInOneAnswer = 2;

    std::array<int, numOfChannels> data{};
    char dataFromSensor[bytesInOneAnswer];
    int16_t tmpBuffer;

    while (true)  // it's bad - need to rewrite it with quit from cycle!
    {

        connector.writeData(QByteArray(requestForData, bytesInRequest));

        connector._serial->waitForReadyRead(waitingTime);

        availBytes = connector._serial->bytesAvailable();

        if (availBytes == needBytesFromSensor)
        {
            connector._serial->read(answerCode, bytesCode);  // read code of answer from the Arduino

            for (size_t i = 0; i < numOfChannels; ++i)
            {
                connector._serial->read(dataFromSensor, bytesInOneAnswer);  // read data from the Arduino & sensor
                memcpy(&tmpBuffer, dataFromSensor, bytesInOneAnswer);
                data[i] = tmpBuffer;
            }
        }
        else
        {
            if (availBytes == 0)
            {
                std::cout << "No bytes available\n";
            }
            else
            {
                std::cout << "Bytes available: " << availBytes << "\nIt's wrong!";
                std::cin.get();
                return 3;
            }
            std::cin.get();
            return 4;
        }

        for (const auto elem : data)
        {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }

    connector._serial->close();
    std::cin.get();

    return app.exec();
}
