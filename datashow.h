#ifndef DATASHOW_H
#define DATASHOW_H

#include <QMainWindow>
#include "heartchart.h"
#include "report.h"
#include <QBluetoothSocket>
#include "handlesql.h"
#include "csvwriter.h"
#include "devicecharts.h"

extern Report r;
extern HandleSQL m;
namespace Ui {
class DataShow;
}

#define PKG_LENGTH      7
#define MAX_LENGTH      256
#define BIT0            0x01
#define BIT1            0x02
#define BIT2            0x04
#define BIT3            0x08
#define BIT4            0x10
#define BIT5            0x20
#define BIT6            0x40
#define BIT7            0x80

class DataShow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataShow(QWidget *parent = nullptr,int axisXMax=500,int axisYMax1=500,int axisYMax2=100,QString label="",QBluetoothSocket*socketparent=nullptr,CsvWriter *writer=nullptr);
    ~DataShow();
    void append(DeviceData data);
private slots:
    void GetFeature_clicked();
    void SendCommand_clicked();
public:
    CsvWriter *c;
    DeviceChart *devicechart;
    QBluetoothSocket *socket;
    QVBoxLayout *verticalLayoutPatient;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *Spo2label;
    QLabel *Patientlabel;
    QPushButton *GetFeature;
    QPushButton *SendCommand;
    QSpacerItem *horizontalSpacer;
    QLabel *pulseRatelabel;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QLabel *Spo2;
    QSpacerItem *horizontalSpacer_4;
    QLabel *pulseRate;
    QString label;
    int index;
//    HeartChart *chart;
    Ui::DataShow *ui;
};

#endif // DATASHOW_H
