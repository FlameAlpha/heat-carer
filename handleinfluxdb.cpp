#include "handleinfluxdb.h"

HandleInfluxDB::HandleInfluxDB(QWidget * parent):
    wait(parent)
{
    handleinfluxdb = influxdb::InfluxDBFactory::Get("http://admin:admin@localhost:8086/write?db=mydb");
}
bool HandleInfluxDB::InsertData(QString name, DeviceData data){
    if(!name.contains("DV")){
        handleinfluxdb->write(influxdb::Point{ name.toStdString() }
                .addTag("ID", data.address.toString().toStdString())
                .addField("DATETIME", data.time)
                .addField("SIGNAL", data.signal)
                .addField("PULSERATE", data.pulseRate)
                .addField("SPO2", data.spo2Sat)
                .addField("BAR", data.bar)
                .addField("PLETH", data.pleth)
        );
    }
    else{
        handleinfluxdb->write(influxdb::Point{ name.toStdString() }
                .addTag("ID", data.address.toString().toStdString())
                .addField("DATETIME", data.time)
                .addField("ECG", data.signal)
                .addField("ACCX", data.pulseRate)
                .addField("ACCY", data.spo2Sat)
                .addField("ACCZ", data.bar)
                .addField("HR", data.pleth)
        );
    }
    return true;
}
bool HandleInfluxDB::InsertData(QString name, QList<DeviceData> alldata){
    handleinfluxdb->batchOf(alldata.size());
    if(lastdata.time >= alldata[0].time)
    {
        alldata[0].time = lastdata.time + 1;
    }
    lastdata.time = alldata[0].time + alldata.size() - 1;
    if(!name.contains("DV")){
        int i = 0;
        for (auto data:alldata)
            handleinfluxdb->write(influxdb::Point{ name.toStdString() }
                    .addTag("ID", data.address.toString().toStdString())
                    .addField("DATETIME", alldata[0].time+(++i)*5)
                    .addField("SIGNAL", data.signal)
                    .addField("PULSERATE", data.pulseRate)
                    .addField("SPO2", data.spo2Sat)
                    .addField("BAR", data.bar)
                    .addField("PLETH", data.pleth)
            );
    }
    else{
        int i = 0;
        for (auto data:alldata)
            handleinfluxdb->write(influxdb::Point{ name.toStdString() }
                    .addTag("ID", data.address.toString().toStdString())
                    .addField("DATETIME", alldata[0].time+(++i)*5)
                    .addField("ECG", data.signal)
                    .addField("ACCX", data.pulseRate)
                    .addField("ACCY", data.spo2Sat)
                    .addField("ACCZ", data.bar)
                    .addField("HR", data.pleth)
            );
    }
    return true;
}
