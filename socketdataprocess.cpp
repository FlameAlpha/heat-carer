#include "socketdataprocess.h"
#define unsigned char uchar
QList<int> realtime_Ecg_Decode(QByteArray package)
{
    int packageLength = (int)(float(package.size() / 2.0));
    //qDebug()<<"Ecg Decode "<<packageLength;
    QList<int> datalist;
    for (int i = 0; i < packageLength; i++)
    {
        int data=0;
        if ((package[2 * i] & 0x80) != 0)
        {
            data |= 0xFF << 24;
            data |= 0xFF << 16;
            data |= 0xF0 << 8;
        }
        data |=  ((package[2 * i] & 0xff)<<8);
        data |=  (package[2 * i + 1] & 0xff);
        datalist.push_back(data);
    }
    return datalist;
}

QList<int>  realtime_Acc_Decode(QByteArray package)
{
    //qDebug()<<"Acc Decode";
    QList<int> datalist;
    int packageLength = (int)(float(package.size() / 2.0));
    for (int i = 0; i < packageLength; i++)
    {
        int data=0;
        data |= (package[2 * i + 1]& 0xff) << 8;
        data |= (package[2 * i]& 0xff);
        data >>= 6;

        if ((package[2 * i + 1] & 0x80) != 0)
        {
            data |= 0xFF << 24;
            data |= 0xFF << 16;
            data |= 0xFC << 8;
        }
        datalist.push_back(data);
    }
    return datalist;
}
// Real-Time HeartRate & Battery
QList<int> realtime_HeartRate_Decode(QByteArray package)
{
    //qDebug()<<"HeartRate Decode";
    QList<int> datalist;
    //little endian
    datalist.push_back(((package[1]& 0xff) << 8) | (package[0]& 0xff));
    return datalist;
}
QList<int> realtime_Battery_Decode(QByteArray package)
{
    //qDebug()<<"Battery Decode";
    QList<int> datalist;
    //little endian
    datalist.push_back(((package[1]& 0xff) << 8) | (package[0]& 0xff));
    return datalist;
}

QList<int> decodeSwitch(int packageType,QByteArray package)
{
    switch (packageType)
    {
        case 0:
            return realtime_Ecg_Decode(package);
        case 2:
            return realtime_Acc_Decode(package);
        case 5:
            return realtime_HeartRate_Decode(package);
        case 11:
            return realtime_Battery_Decode(package);
        default:
            QList<int> temp;
            return temp;
    }
}

