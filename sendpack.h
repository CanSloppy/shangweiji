#ifndef SENDPACK_H
#define SENDPACK_H
#include <QThread>
#include <QtSerialPort/QSerialPort>


class SendPack : public QThread
{
    Q_OBJECT
public:
    SendPack(QSerialPort *port);
    void run() override;

private:
    QSerialPort *m_serialport;

    bool makePack(char cmd,char param,char data[],char *pack);
};

#endif // SENDPACK_H
