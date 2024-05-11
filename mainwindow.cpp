#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Protocal.h"
#include <QMessageBox>
#include <QTimer>
#include <QProgressBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::monitor()
{
    //串口初始化
    serialPort = new QSerialPort;
    //设置使用哪个串口
    serialPort->setPortName("COM2");
    //打开串口
    if(!serialPort->open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(nullptr,
                              "串口打开失败",
                              "串口COM2不存在或者已被占用！",
                              QMessageBox::Ok);
        QTimer::singleShot(0,qApp,SLOT(quit()));
    }
    serialPort->setBaudRate(19200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    senderThread = new SendPack(serialPort);
    senderThread->start();  //启动线程

    receiverThread = new ReceivePack(serialPort);
    receiverThread->start();
    //添加信号槽:接收线程发信号
    connect(receiverThread,SIGNAL(receiveResopnsePack(char,char,int)),
            this,SLOT(updateUI(char, char, int)));
}

void MainWindow::updateUI(char cmd, char param, int data)
{
    switch (cmd) {
    case ZHU_JI_WEN_DU_RSP:
    {
        ui->label_wendu->setText(QString::number(data));
        break;
    }
    case ZHOU_SU_DU_RSP:
    {
        QLabel *labels[2] = {ui->labelMainCircle,ui->labelDMainCircle};
        if(param !=1 && param!=2) break;
        labels[param-1]->setText(QString::number(data));
        break;
     }
    case GANG_YOU_LIANG_RSP:
    {
        QLabel *labels  [3] = {ui->label_oilNum1,ui->label_oilNum2,ui->label_oilNum3};
        if(param <1 || param >3)break;
        labels[param-1]->setText(QString::number(data)+" L");
        break;
    }
    case YA_LI_CANG_RSP:
    {
        QLabel *labels [5] = {ui->labelStorePa1,ui->labelStorePa2,ui->labelStorePa3,ui->labelStorePa4,ui->labelStorePa5};
        QProgressBar *bars [5] = {ui->progressBar_cang1,ui->progressBar_cang2,ui->progressBar_cang3,ui->progressBar_cang4,ui->progressBar_cang5};
        if(param <1 || param >5)break;
        labels[param-1]->setText(QString::number(data)+" Pa");
        bars[param-1]->setValue(data);
        break;
    }
    case JI_XIE_BI_RSP:
    {
        ui->labelArmPress->setText(QString::number(data)+" Pa");
        ui->progressBar_arm->setValue(data);
        break;
    }

    }
}

