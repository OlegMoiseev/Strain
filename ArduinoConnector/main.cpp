#include <iostream>
#include <array>

#include <QtCore>
#include <QDebug>

#include "arduino.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    Arduino connector(Arduino::identPort());
    std::cin.get();
    connector.getPeriod();

    return app.exec();
}

/*constexpr char requestForData[] = { 1, 0, 0, 0, 0 };
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

    if (availBytes == 13)
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
}*/
