#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include "sendpack.h"
#include "receivepack.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void monitor();

private slots:
    void updateUI(char cmd,char param,int data);

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;    //串口
    SendPack *senderThread;     //串口发送线程
    ReceivePack *receiverThread;//串口接收线程
};
#endif // MAINWINDOW_H
