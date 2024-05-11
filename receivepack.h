#ifndef RECEIVEPACK_H
#define RECEIVEPACK_H
#include <QThread>
#include <QtSerialPort/QSerialPort>


class ReceivePack : public QThread
{
    Q_OBJECT
public:
    ReceivePack(QSerialPort *port);
    void run() override;

signals:
    void receiveResopnsePack(char cmd,char param,int data);

private:
    QSerialPort *m_serialport;

    bool parsePack(unsigned char pack[],int size,char *cmd,char *param,int *data);
};

#endif // RECEIVEPACK_H
