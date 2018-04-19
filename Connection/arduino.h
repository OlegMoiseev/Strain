#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort/QSerialPort>
#include <QtCore/QtCore>

class Arduino : public QObject
{
    //Q_OBJECT

private:
    QByteArray _recievedData;

public:
    QSerialPort* _serial;

    explicit Arduino(const QString &portName);

    void writeData(QByteArray data) const;

    static QString identPort();

public slots:

    void readData(char* buf, int bytes) const;
};
#endif // ARDUINO_H
