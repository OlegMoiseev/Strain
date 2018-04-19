#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort>
#include <QtCore>

#include <array>


class Arduino : public QObject
{
    Q_OBJECT

private:
    QSerialPort* _serial;

    QByteArray _answerCode;
    QByteArray _periodRaw;

    //char* _answerCode;
    //char* _periodRaw;

    int _bytes;

    uint32_t _period;

    char *_requestForPeriod;
    char *_requestForData;




public:
    explicit Arduino(const QString &portName);

    static QString identPort();
    void writeData(QByteArray data) const;

signals:
    void getPeriod();
    void getData();

public slots:
    void read();

    void readPeriod() const;
    void readData() const;



};
#endif // ARDUINO_H
