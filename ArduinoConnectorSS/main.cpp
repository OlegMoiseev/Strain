#include <QCoreApplication>

#include "arduino.h"

#include <thread>
#include <chrono>
#include <iostream>

void foo(Arduino &ar)
{
    std::cout << "start waiting\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "stop wait\n";
    auto ans = ar.getPeriod();
    std::cout << "Period from thread: " << ans << '\n';
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Arduino arduino("/dev/cu.usbmodem1421");
    std::thread t1(foo, std::ref(arduino));
    t1.detach();
    std::cout << "Thread was detached!\n";
    return a.exec();
}
