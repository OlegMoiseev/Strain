#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort>
#include <QtCore>

#include <array>
#include <atomic>

class Arduino : public QObject
{
    Q_OBJECT

private:
    QSerialPort* _serial;

    QByteArray _answerCode;

    std::array<QByteArray, 6> _data;

    char *_requestForPeriod;
    char *_requestForData;

    uint32_t _period;

public:
    explicit Arduino(const QString &portName);
    std::atomic<bool> _dataReady;

    static QString identPort();
    static uint32_t dataToUint32(QByteArray data);

    void writeData(const QByteArray &data) const;

    uint32_t getPeriod();
    std::array<int, 6> getData();

public slots:
    void read();
};
#endif // ARDUINO_H
