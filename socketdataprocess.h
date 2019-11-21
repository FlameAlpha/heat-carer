#ifndef SOCKETDATAPROCESS_H
#define SOCKETDATAPROCESS_H
#include "datashow.h"
#include "handlesql.h"
#include <QObject>
#include <QDateTime>
#include <QWidget>
#include <QListWidgetItem>
#include <QtBluetooth/qbluetoothglobal.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothsocket.h>
#include <QGraphicsScene>
#include <QPointF>
#include <QPainterPath>
#include <QTextBrowser>
#include <QMessageBox>
#include "keyin.h"
#include <mutex>
#include <thread>
#include <sys/timeb.h>
#include "csvwriter.h"
#include "handleinfluxdb.h"

extern HandleInfluxDB infux;
extern std::mutex mt;
# define period 256
static const QString serviceUuid("00001101-0000-1000-8000-00805f9b34fb");
extern HandleSQL m;
QList<int> decodeSwitch(int packageType,QByteArray package);
class SocketDataProcess : public QObject
{
public:
    Q_OBJECT
public:
    QBluetoothSocket *socket;
    QBluetoothAddress address;
    QTextBrowser *textBrowser_info;
    QWidget *WindowsParent;
    DataShow *showWindows;
    QString serviceUuid;
    QString tablename;
    QList<DeviceData> datalist;
    DeviceInfo deviceinfo;
    KeyIn *keyin;
    QString name;
    bool isConnect;
    bool havehead = false;
    QByteArray command;
    QList<QByteArray> linelist;
    timeb currenttime;
    CsvWriter c;
    QByteArray btData;
    std::vector<int> btPackageLength = { 32, 32, 24, 6, 0, 2, -2, -2, -2, -2, -2, 2, -2, -2, -2 }; // -1: 無資料, -2: 不限定
    QList<int> TypeUndefineLength = { 6, 7, 8, 9, 10, 12, 13, 14 };
    SocketDataProcess(QTextBrowser *textBrowser_info = nullptr,QWidget *WindowsParent = nullptr,QObject *parent = nullptr):QObject(parent),textBrowser_info(textBrowser_info),WindowsParent(WindowsParent){
        isConnect = false;
        qRegisterMetaType< DeviceData >("DeviceData");
        keyin = new KeyIn(WindowsParent);
        infux.handleinfluxdb->batchOf(period);
    }
    ~SocketDataProcess()
    {
        delete showWindows;
        delete socket;
        delete keyin;
    }
    void init()
    {
        if(this->name.contains("DV")){
            showWindows = new DataShow(nullptr,1000,100,3000,this->name,socket,&c);
        }
        else {
            showWindows = new DataShow(nullptr,500,25,100,this->name,socket,&c);
        }
        connect(socket,
                SIGNAL(readyRead()),
                this,
                SLOT(readBluetoothDataEvent())
                );
        connect(socket,
                SIGNAL(connected()),
                this,
                SLOT(bluetoothConnectedEvent())
                );
        connect(socket,
                SIGNAL(disconnected()),
                this,
                SLOT(bluetoothDisconnectedEvent())
                );
        connect(this,
                SIGNAL(isObtainDevice(DeviceData)),
                this,
                SLOT(showdata(DeviceData))
                );
        if(!m.isObtainPatientDeviceInfo(address.toString())){
            keyin->deviceinfo.name = name;
            keyin->deviceinfo.address = address.toString();
            keyin->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
            //keyin->setModal(true);
            keyin->show();
            connect(keyin,
                    SIGNAL(isObtain(DeviceInfo)),
                    this,
                    SLOT(UserConnect(DeviceInfo))
                    );
        }
        else {
            UserConnect(m.GetPatientDeviceInfo(address.toString()));
        }
    }
    void ParsePackage(QByteArray pkgData,int packageType=0)
    {

        if(!this->name.contains("DV")){
            DeviceData devicedata;
            devicedata.ifpleth = true;
            devicedata.ifbar = true;
            devicedata.ifsignal = true;
            devicedata.ifspo2Sat = true;
            devicedata.ifpulseRate = true;
            devicedata.name = this->name;
            devicedata.signal = pkgData[0] & (BIT0|BIT1|BIT2|BIT3);
            devicedata.pleth = pkgData[1] & (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6);
            devicedata.bar = pkgData[2] & (BIT0|BIT1|BIT2|BIT3);
            devicedata.pulseRate = (pkgData[3] & (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6)) | ((pkgData[2] & BIT6) << 1);
            devicedata.spo2Sat = pkgData[4] & (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6);
            ftime(&currenttime);
            devicedata.time = currenttime.time*1000+currenttime.millitm;
            devicedata.address = address;
            infux.InsertData(tablename,devicedata);
            datalist.push_back(devicedata);
            if(datalist.size() >= period) {
                c.InsertData(datalist);
                m.InsertData(tablename,datalist);
                datalist.clear();
            }
            //emit(isObtainDevice(devicedata));
            showWindows->append(devicedata);
        }
        else{
            DeviceData devicedata;
            devicedata.name = this->name;
            ftime(&currenttime);
            devicedata.time = currenttime.time*1000+currenttime.millitm;
            devicedata.address = address;
            auto datapkglist = decodeSwitch(packageType,pkgData);
            if(packageType==0)
                for(auto data:datapkglist)
                {
                    devicedata.ifECG = true;
                    devicedata.ECG = data;
                    infux.InsertData(tablename,devicedata);
                    datalist.push_back(devicedata);
                    if(datalist.size() >= period) {
                        c.InsertData(datalist);
                        m.InsertData(tablename,datalist);
                        infux.InsertData(tablename,datalist);
                        datalist.clear();
                    }
                    //emit(isObtainDevice(devicedata));
                    //devicedata.pulseRate = data;
                    showWindows->append(devicedata);
                }
            else if(packageType==5)
                for(auto data:datapkglist)
                {
                    //devicedata.pulseRate = data;
                    devicedata.ifheartRate = true;
                    devicedata.heartRate = data;
                    infux.InsertData(tablename,devicedata);
                    datalist.push_back(devicedata);
                    if(datalist.size() >= period) {
                        m.InsertData(tablename,datalist);
                        infux.InsertData(tablename,datalist);
                        datalist.clear();
                    }
                    showWindows->append(devicedata);
                    //qDebug()<<data;
                }
        }
        //qDebug()<< address << devicedata.time << devicedata.spo2Sat << devicedata.pulseRate << devicedata.signal;
    }
    void DataProcess(){
        QByteArray line = linelist.first();
        linelist.pop_front();
        if(!this->name.contains("DV")){
            if(!line.isEmpty()){
                for (int i = 0; i < line.size(); i++) {
                      if(line[i]& BIT7){
                          havehead = true;
                          //textBrowser_info->append(QString(command.toHex(0)));
                          if((command.size()==7)||(command.size()==5))
                                this->ParsePackage(command);
                          command.clear();
                      }
                      if(havehead)
                        command.push_back(line[i]);
                  }
            }
        }
        else{
            if(this->name.contains("DV"))
                if(!line.isEmpty())
                {
                    btData = btData + line;
                    int startIndex = btData.indexOf(0x53);
                    while (startIndex != -1 && btData.size() > (3 + startIndex))
                    {
                        int dataType = btData[startIndex + 2];
                        // Check Package Length (Type: 0,1,2,3,4,5), Uncheck Package Length (Type: 6,7,8,A)
                        if (dataType < 15)
                        {
                            if (int(btData[startIndex + 1]) == btPackageLength[dataType] || TypeUndefineLength.indexOf(dataType) != -1)
                            {
                                int dataLength = btData[startIndex + 1];
                                bool isInBound = ((startIndex + dataLength + 3) < btData.size());
                                bool isLastByteOK = false;
                                if (isInBound)
                                {
                                    isLastByteOK = (btData[startIndex + dataLength + 3] == char(0x50));
                                }
                                if (!isInBound)
                                {
                                    break;
                                }
                                else{
                                    if (!isLastByteOK)
                                    {
                                        startIndex = btData.indexOf(0x53, startIndex + 1);
                                    }
                                    else
                                    {
                                        ParsePackage(btData.mid(startIndex + 3, dataLength),dataType);
                                        btData.remove(startIndex, dataLength + 4);
                                    }
                                }
                            }
                            else
                            {
                                startIndex = btData.indexOf(0x53, startIndex + 1);
                            }
                        }
                        else
                        {
                            startIndex = btData.indexOf(0x53, startIndex + 1);
                        }
                        //Do Until No StartIndex Is Found
                    }
                }
        }
    }
    void ProcessMultiThread()
    {
        while(1)
        {
            if(!linelist.isEmpty()&&socket->state()==3){
                mt.lock();
                DataProcess();
                mt.unlock();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
signals:
    void isObtainDevice(DeviceData devicedata);
public slots:
    void UserConnect(DeviceInfo deviceinfo)
    {
        this->deviceinfo = deviceinfo;
        if(this->name.contains("DV")){
            socket->connectToService(address, QBluetoothUuid(serviceUuid) ,QIODevice::ReadWrite);
            qDebug()<<"connecting ! ";
        }
        else{
            socket->connectToService(address, QBluetoothUuid(QBluetoothUuid::SerialPort) ,QIODevice::ReadWrite);
            qDebug()<<"connecting ! ";
        }
        if(m.CreatTable(this->tablename))
            qDebug()<<"creat successful ! ";
        else {
            qDebug()<<"creat error ! ";
        }

        showWindows->setWindowTitle(this->tablename+" Patient : "+deviceinfo.patientID);
        //qDebug()<<"try to connect!";
    }
    void showdata(DeviceData devicedata)
    {
        showWindows->append(devicedata);
    }
    void readBluetoothDataEvent()
    {
        mt.lock();
        linelist.append(socket->readAll());
        mt.unlock();
    }
    void bluetoothConnectedEvent()
    {
        isConnect = true;
        if(this->name.contains("DV")){
            qDebug()<<"send command!";
            socket->write("S VIEW AFE ON\r\n");
        }
        showWindows->show();
        QMessageBox::information(WindowsParent,tr("Info"),tr("successful connection!"));
        std::thread t(&SocketDataProcess::ProcessMultiThread,this);
        t.detach();
    }
    void bluetoothDisconnectedEvent()
    {
        isConnect = false;
        showWindows->close();
        QMessageBox::information(WindowsParent,tr("Info"),tr("successful disconnection!"));
    }

};
#endif // SOCKETDATAPROCESS_H
