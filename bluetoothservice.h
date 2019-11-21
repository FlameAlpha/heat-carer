#ifndef BLUETOOTHSERVICE_H
#define BLUETOOTHSERVICE_H

#include <QWidget>
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
#include "socketdataprocess.h"
#include <QMetaType>
#include <QUuid>

namespace Ui {
class BlueToothService;
}

class BlueToothService : public QWidget
{
    Q_OBJECT

public:
    explicit BlueToothService(QWidget *parent = nullptr);
    ~BlueToothService();

private slots:
    void on_pushButton_scan_clicked();
    void addBlueToothDevicesToList(const QBluetoothDeviceInfo&);
    void addBlueToothDevicesToList(QList<QString> devicelist);
    void on_pushButton_openBluetooth_clicked();
    void on_pushButton_closeDevice_clicked();
    void itemActivated(QListWidgetItem *item);
    void readBluetoothDataEvent();
    void bluetoothConnectedEvent();
    void bluetoothDisconnectedEvent();
    void on_pushButton_disconnect_clicked();
    void send_data(QString string);
    void on_pushButton_show_clicked();
    void on_pushButton_about_clicked();

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    QList<SocketDataProcess*> socketlist;
private:
    Ui::BlueToothService *ui;
};

#endif // BLUETOOTHSERVICE_H
