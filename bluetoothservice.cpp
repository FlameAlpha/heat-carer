#include "bluetoothservice.h"
#include "ui_bluetoothservice.h"

//static const QString serviceUuid("a04b8c3f-6eb9-4c9d-9c13-2f85ff05fb65");

BlueToothService::BlueToothService(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlueToothService)
{
    ui->setupUi(this);
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    localDevice = new QBluetoothLocalDevice();
    ui->pushButton_show->setEnabled(false);
    if( localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff ) {
        ui->pushButton_openBluetooth->setEnabled(true);
        ui->pushButton_closeDevice->setEnabled(false);
    }else {
        ui->pushButton_openBluetooth->setEnabled(false);
        ui->pushButton_closeDevice->setEnabled(true);
    }

    if( localDevice->hostMode() == QBluetoothLocalDevice::HostDiscoverable ) {
        ui->checkBox_discoverable->setChecked(true);
    }else {
        ui->checkBox_discoverable->setChecked(false);
    }

    connect(discoveryAgent,
            SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this,
            SLOT(addBlueToothDevicesToList(QBluetoothDeviceInfo))
            );
    connect(ui->list,
            SIGNAL(itemActivated(QListWidgetItem*)),
            this,
            SLOT(itemActivated(QListWidgetItem*))
            );
    qRegisterMetaType< QList<QString> >("QList<QString>");
//    connect(&w,
//            SIGNAL(isObtain(QList<QString>)),
//            this,
//            SLOT(addBlueToothDevicesToList(QList<QString>))
//            );
    ui->pushButton_openBluetooth->setEnabled(false);
    ui->pushButton_closeDevice->setEnabled(false);
    this->setWindowTitle("BlueTooth Selecter");
    ui->textBrowser_info->setVisible(false);
}


BlueToothService::~BlueToothService()
{
    delete discoveryAgent;
    delete localDevice;
    delete ui;
    qDeleteAll(socketlist.begin(),socketlist.end());
    socketlist.clear();
}
void BlueToothService::addBlueToothDevicesToList(QList<QString> devicelist)
{
    ui->pushButton_scan->setEnabled(true);
    for(auto label:devicelist)
    {
        int index = label.indexOf(' ');
        if (index == -1)
            return;
        QBluetoothAddress address(label.left(index));
        QList<QListWidgetItem *> items = ui->list->findItems(label, Qt::MatchExactly);

        if (items.empty()) {
            QListWidgetItem *item = new QListWidgetItem(label);
            QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(address);
            if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
                item->setTextColor(QColor(Qt::green));
            else
                item->setTextColor(QColor(Qt::black));
            ui->list->addItem(item);
        }
    }
}
void BlueToothService::on_pushButton_scan_clicked()
{
    ui->pushButton_scan->setEnabled(false);
    discoveryAgent->start();
    ui->pushButton_show->setEnabled(true);
    //w.SearchBluetooth();
}

void BlueToothService::addBlueToothDevicesToList( const QBluetoothDeviceInfo &info )
{
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());

    QList<QListWidgetItem *> items = ui->list->findItems(label, Qt::MatchExactly);

    if (items.empty()) {
        QListWidgetItem *item = new QListWidgetItem(label);
        QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
        if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
            item->setTextColor(QColor(Qt::green));
        else
            item->setTextColor(QColor(Qt::black));
        ui->list->addItem(item);
    }
}

void BlueToothService::on_pushButton_openBluetooth_clicked()
{
    localDevice->powerOn();
    ui->pushButton_closeDevice->setEnabled(true);
    ui->pushButton_openBluetooth->setEnabled(false);
    ui->pushButton_scan->setEnabled(true);
}


void BlueToothService::on_pushButton_closeDevice_clicked()
{
    localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    ui->pushButton_closeDevice->setEnabled(false);
    ui->pushButton_openBluetooth->setEnabled(true);
    ui->pushButton_scan->setEnabled(false);
}

void BlueToothService::itemActivated(QListWidgetItem *item)
{
    QString text = item->text();
    int index = text.indexOf(' ');
    if (index == -1)
        return;
    QBluetoothAddress address(text.left(index));
    QString name(text.mid(index + 1));
    for(auto sockettemp:socketlist){
        if(sockettemp->isConnect==false)
        {
            QMessageBox::information(this,tr("Info"),tr("The last device is connecting, waiting..."));
            return;
        }
        if(sockettemp->address == address)
        {
            if(sockettemp->isConnect)
                QMessageBox::information(this,tr("Info"),tr("The device is connected ! "));
            else
                QMessageBox::information(this,tr("Info"),tr("The device is connecting ! "));
            return;
        }
    }
    QMessageBox::information(this,tr("Info"),tr("The device is connecting..."));
    SocketDataProcess* socket = new SocketDataProcess(ui->textBrowser_info,this);
    socket->address = address;
    socket->name = name;
    socket->tablename = name + "("+ address.toString() +")";
    socket->serviceUuid = serviceUuid;
    socket->socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    socket->init();
    socketlist.push_back(socket);
}

void BlueToothService::readBluetoothDataEvent()
{
}

void BlueToothService::bluetoothConnectedEvent()
{
    QMessageBox::information(this,tr("Info"),tr("successful connection!"));
    ui->pushButton_show->setEnabled(true);
}

void BlueToothService::bluetoothDisconnectedEvent()
{
    QMessageBox::information(this,tr("Info"),tr("successful disconnection!"));
}

void BlueToothService::on_pushButton_disconnect_clicked()
{
    QString text = ui->list->currentItem()->text();
    int index = text.indexOf(' ');
    if (index == -1)
        return;
    QBluetoothAddress address(text.left(index));
    for(auto socket:socketlist)
    {
        if(socket->address == address)
        {
            socket->socket->disconnectFromService();
            socketlist.removeOne(socket);
            qDeleteAll(socketlist.begin()+socketlist.indexOf(socket),socketlist.begin()+socketlist.indexOf(socket));
            return;
        }
    }
}

void BlueToothService::send_data(QString string)
{
    QByteArray arrayData;
    arrayData = string.toUtf8();
}

void BlueToothService::on_pushButton_about_clicked()
{
    QMessageBox::information(this,tr("About"),tr("HeartCare 3.0 \n"
                                                 "Author : Flame \n"
                                                 "Weibo : Flame_Blog \n"
                                                 "Email : f547453159@outlook.com \n"
                                                 "Blog : https://www.cnblogs.com/FlameBlog \n"
                                                 "Github : https://github.com/FlameAlpha \n"
                                                 "\n"
                                                 "Based on Qt 5.12.0,Built on Sep 1 2019 \n"
                                                 "Copyright 2019 Flame All rights reserved \n"
                                              ));
}

void BlueToothService::on_pushButton_show_clicked()
{
    QString text = ui->list->currentItem()->text();
    int index = text.indexOf(' ');
    if (index == -1)
        return;
    QBluetoothAddress address(text.left(index));
    for(auto socket:socketlist)
    {
        if(socket->address == address)
        {
            socket->showWindows->show();
            return;
        }
    }
    qDebug()<<"This device is not connected !";
}
