﻿#include "receivepack.h"
#include <QDebug>

ReceivePack::ReceivePack(QSerialPort *port)
{
    m_serialport = port;
}

void ReceivePack::run()
{
    unsigned char pack[9];
    char cmd;
    char param;
    int data;
    while(1){
        m_serialport->waitForReadyRead(10);
        int ret = m_serialport->bytesAvailable();
        if(ret == 0) continue;
        ret = m_serialport->read((char*)pack,sizeof(pack));
        qDebug() << "read:" << ret << " bytes." <<
                    pack[0] << " " << pack[1] << "" << pack[2] << " " << pack[3] <<
                    pack[4] << " " << pack[5] << "" << pack[6] << " " << pack[7] <<
                    pack[8];
       if(parsePack((unsigned char*)pack,sizeof(pack),&cmd,&param,&data))
       {
           qDebug() << "prase suceed!";
           emit receiveResopnsePack(cmd,param,data);
       }else{
           qDebug() << "收到非法包";
       }
    }
}

bool ReceivePack::parsePack(unsigned char pack[], int size, char *cmd, char *param, int *data)
{
    if(!pack || size < 9)return false;
    if(pack[0] == 0xEF && pack[8] == 0xFE){
        //判断1的个数
        int count = 0;
        for(int i = 1;i < 8;i++)
        {
            char tmp = pack[i];
            for(int k = 0;k < 8;k++)
            {
                if(tmp & 1)count ++;
                tmp >>=1;
            }
        }
        if(count %2)return false;
        *cmd = pack[1];
        *param = pack[2];
        *data = pack[3] | (pack[4] << 8) | (pack[5] << 16) | (pack[6]<< 25);
        qDebug() << "data" << *data;
        return true;
    }
    return false;
}
