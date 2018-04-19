#include <QCoreApplication>

#include "arduino.h"

#include <thread>
#include <chrono>
#include <iostream>


void foo(Arduino &ar)
{
    std::cout << "Start waiting...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Stop waiting\n";
    emit ar.getPeriod();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //Arduino arduino("/dev/cu.usbmodem1421");
    Arduino arduino(Arduino::identPort());
    std::thread t1(foo, std::ref(arduino));
    t1.detach();
    return a.exec();
}
