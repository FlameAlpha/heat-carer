#ifndef MEASURESQL_H
#define MEASURESQL_H

#include <QObject>
#include "dataclass.h"
#include "heartchart.h"
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QString>
#include <QThread>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <QDebug>
#include <QWidget>
#define SQLString "QMYSQL"
class HandleSQL : public QObject
{
public:
    Q_OBJECT
public:
    HandleSQL(QObject *parent=nullptr,QString Databasename="hospital",QString HostName="39.105.27.135",int Port=3306,QString UserName="root",QString Password="@neu123#");
    void init();
    void init_self(QWidget * wait_ = nullptr);
    bool CreatTable(QString name);
    bool InsertDataProcessing();
    bool InsertData(QString name, QList<DeviceData> alldata);
    //bool SearchData(QString name,long long time);
    bool InsertPatientDeviceInfo(DeviceInfo info);
    bool isObtainPatientDeviceInfo(QString address);
    DeviceInfo GetPatientDeviceInfo(QString address);
    ~HandleSQL();
    void InsertDataMultiThread(QString name,QList<DeviceData> alldata);


signals:
    void storeData(bool status);
private:
    QWidget * wait;
    bool already = true;
    DeviceData lastdata;
    QSqlDatabase db;
    QList<QString> commandlist;
    QString Databasename;
    int Port;
    QString UserName;
    QString Password;
    QString HostName;
};

#endif // MEASURESQL_H
