#ifndef DATACLASS_H

#define DATACLASS_H
#include <QBluetoothAddress>
#include <QString>

class DeviceInfo
{
public:
    DeviceInfo() {}
    int type = 0;
    QString patientID = "";
    QString name = "";
    QString address = "";
};


class DeviceData{
public:
    int signal=0;
    int pleth=0;
    int bar=0;
    int pulseRate=0;
    int heartRate=0;
    int spo2Sat=0;
    int Accx=0;
    int Accy=0;
    int Accz=0;
    int ECG=0;

    bool ifsignal=0;
    bool ifpleth=0;
    bool ifbar=0;
    bool ifpulseRate=0;
    bool ifheartRate=0;
    bool ifspo2Sat=0;
    bool ifAccx=0;
    bool ifAccy=0;
    bool ifAccz=0;
    bool ifECG=0;

    QString name = "";
    long long time = 0;
    QBluetoothAddress address = QBluetoothAddress("");
};

#endif // DATACLASS_H
