#ifndef MEASUREINFLUXDB_H
#define MEASUREINFLUXDB_H
#include "InfluxDB/include/InfluxDBFactory.h"
#include "InfluxDB/include/Point.h"
#include <QWidget>
#include "dataclass.h"

extern bool initreport;
extern bool initmysql;
class HandleInfluxDB
{
public:
    HandleInfluxDB(QWidget * parent=nullptr);
    bool InsertData(QString name, DeviceData data);
    bool InsertData(QString name, QList<DeviceData> alldata);
    std::unique_ptr<influxdb::InfluxDB> handleinfluxdb;
private:
    QWidget * wait;
    DeviceData lastdata;
};

#endif // MEASUREINFLUXDB_H
