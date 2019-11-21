#include "datashow.h"
#include "ui_datashow.h"

DataShow::DataShow(QWidget *parent, int axisXMax, int axisYMax1, int axisYMax2, QString label, QBluetoothSocket*socketparent, CsvWriter *writer) :
    QMainWindow(parent),
    c(writer),
    socket(socketparent),
    ui(new Ui::DataShow)
{
    ui->setupUi(this);
    //(1260, 738)  630*369
    index = 0;
    //chart = new HeartChart(axisXMax,axisYMax1,axisYMax2);

    this->label = label;
    qDebug()<<"DataShow init ! ";
    if(label.contains("DV")){
        devicechart = new DeviceChart(Devices::DV);
        this->resize(1260, 369);
        //ui->gridLayout->addLayout(verticalLayout, 0, 0, 1, Qt::AlignLeft);
        //chart->Pulsechart->setTitle("心电");
        ui->gridLayout->setSpacing(2);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(15);
        verticalLayoutPatient = new QVBoxLayout();
        verticalLayoutPatient->setSpacing(4);
        verticalLayoutPatient->setObjectName(QString::fromUtf8("verticalLayoutPatient"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        Patientlabel = new QLabel(ui->centralWidget);
        Patientlabel->setObjectName(QString::fromUtf8("pulseRatelabel"));
        //sizePolicy.setHeightForWidth(Patientlabel->sizePolicy().hasHeightForWidth());
        Patientlabel->setSizePolicy(sizePolicy);
        Patientlabel->setMaximumSize(QSize(16777215, 16777215));
        Patientlabel->setFont(font1);

        GetFeature = new QPushButton("Report",ui->centralWidget);
        //sizePolicy.setHeightForWidth(GetFeature->sizePolicy().hasHeightForWidth());
        GetFeature->setSizePolicy(sizePolicy);
        GetFeature->setMinimumSize(QSize(100, 0));
        GetFeature->setMaximumSize(QSize(16777215, 16777215));
        GetFeature->setFont(font1);

        SendCommand = new QPushButton("SendCommand",ui->centralWidget);
        //sizePolicy.setHeightForWidth(SendCommand->sizePolicy().hasHeightForWidth());
        SendCommand->setSizePolicy(sizePolicy);
        SendCommand->setMinimumSize(QSize(100, 0));
        SendCommand->setMaximumSize(QSize(16777215, 16777215));
        SendCommand->setFont(font1);
        SendCommand->setVisible(false);

        horizontalLayout_1 = new QHBoxLayout();
        horizontalLayout_1->setSpacing(1);
        horizontalLayout_1->setObjectName(QString::fromUtf8("horizontalLayout_1"));

        verticalLayoutPatient->addWidget(Patientlabel,0,Qt::AlignCenter);
        //verticalLayoutPatient->addWidget(chart->LinechartView,1,Qt::AlignLeft);
        verticalLayoutPatient->addWidget(devicechart->ReportChart->chartView,1,Qt::AlignLeft);
        horizontalLayout_1->addWidget(GetFeature,1,Qt::AlignRight);
        //horizontalLayout_1->addWidget(SendCommand,2,Qt::AlignRight);
        verticalLayoutPatient->addLayout(horizontalLayout_1, 0);
        //ui->gridLayout->addWidget(chart->PulsechartView,0,0,Qt::AlignLeft);
        ui->gridLayout->addWidget(devicechart->HeartRateChart->chartView,0,0,Qt::AlignLeft);
        ui->gridLayout->addLayout(verticalLayoutPatient, 0, 1, Qt::AlignLeft);
        //ui->gridLayout->addWidget(chart->chartList[0],1,1,Qt::AlignLeft);
        //ui->gridLayout->addWidget(chart->chartList[1],1,0,Qt::AlignLeft);
        connect(GetFeature,&QPushButton::pressed,this,&DataShow::GetFeature_clicked);
        connect(SendCommand,&QPushButton::pressed,this,&DataShow::SendCommand_clicked);
    }
    else if(label.contains("BerryMed")){
        devicechart = new DeviceChart(Devices::BerryMed);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        Spo2label = new QLabel(ui->centralWidget);
        Spo2label->setObjectName(QString::fromUtf8("Spo2label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        //sizePolicy.setHeightForWidth(Spo2label->sizePolicy().hasHeightForWidth());
        Spo2label->setSizePolicy(sizePolicy);
        Spo2label->setMinimumSize(QSize(50, 0));
        Spo2label->setMaximumSize(QSize(16777215, 80));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(22);
        font.setBold(false);
        font.setWeight(50);
        Spo2label->setFont(font);
        horizontalLayout_2->addWidget(Spo2label);
        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
        horizontalLayout_2->addItem(horizontalSpacer);
        pulseRatelabel = new QLabel(ui->centralWidget);
        pulseRatelabel->setObjectName(QString::fromUtf8("pulseRatelabel"));
        //sizePolicy.setHeightForWidth(pulseRatelabel->sizePolicy().hasHeightForWidth());
        pulseRatelabel->setSizePolicy(sizePolicy);
        pulseRatelabel->setMaximumSize(QSize(16777215, 50));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(22);
        pulseRatelabel->setFont(font1);
        horizontalLayout_2->addWidget(pulseRatelabel);
        verticalLayout->addLayout(horizontalLayout_2);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
        horizontalLayout->addItem(horizontalSpacer_3);
        Spo2 = new QLabel(ui->centralWidget);
        Spo2->setObjectName(QString::fromUtf8("Spo2"));
        //sizePolicy.setHeightForWidth(Spo2->sizePolicy().hasHeightForWidth());
        Spo2->setSizePolicy(sizePolicy);
        Spo2->setMinimumSize(QSize(40, 0));
        Spo2->setMaximumSize(QSize(16777215, 50));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(36);
        font2.setBold(false);
        font2.setWeight(50);
        Spo2->setFont(font2);
        horizontalLayout->addWidget(Spo2);
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
        horizontalLayout->addItem(horizontalSpacer_4);
        pulseRate = new QLabel(ui->centralWidget);
        pulseRate->setObjectName(QString::fromUtf8("pulseRate"));
        //sizePolicy.setHeightForWidth(pulseRate->sizePolicy().hasHeightForWidth());
        pulseRate->setSizePolicy(sizePolicy);
        pulseRate->setMinimumSize(QSize(40, 0));
        pulseRate->setMaximumSize(QSize(16777215, 50));
        pulseRate->setFont(font2);
        horizontalLayout->addWidget(pulseRate);
        verticalLayout->addLayout(horizontalLayout);
        //ui->gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);
        this->setWindowTitle(QApplication::translate("DataShow", "DataShow", nullptr));
        Spo2label->setText(QApplication::translate("DataShow", "\350\241\200\346\260\247(SpO2)", nullptr));
        pulseRatelabel->setText(QApplication::translate("DataShow", "\350\204\211\347\216\207(PR)", nullptr));
        Spo2->setText(QApplication::translate("DataShow", "0", nullptr));
        pulseRate->setText(QApplication::translate("DataShow", "0", nullptr));
        ui->gridLayout->setSpacing(6);
        ui->gridLayout->addLayout(verticalLayout, 0, 0, 1, Qt::AlignLeft);
//        ui->gridLayout->addWidget(chart->PulsechartView,0,1,Qt::AlignLeft);
//        ui->gridLayout->addWidget(chart->BarchartView,1,1,Qt::AlignLeft);
//        ui->gridLayout->addWidget(chart->AreachartView,1,0,Qt::AlignLeft);
        ui->gridLayout->addWidget(devicechart->PulseLineChart->chartView,0,1,Qt::AlignLeft);
        ui->gridLayout->addWidget(devicechart->PulseBarChart->chartView,1,0,Qt::AlignLeft);
        ui->gridLayout->addWidget(devicechart->PulseAreaChart->chartView,1,1,Qt::AlignLeft);
    }
}

DataShow::~DataShow()
{
    delete verticalLayoutPatient;
    delete verticalLayout;
    delete horizontalLayout_1;
    delete horizontalLayout_2;
    delete Spo2label;
    delete Patientlabel;
    delete GetFeature;
    delete SendCommand;
    delete horizontalSpacer;
    delete pulseRatelabel;
    delete horizontalLayout;
    delete horizontalSpacer_3;
    delete Spo2;
    delete horizontalSpacer_4;
    delete pulseRate;
    delete ui;
}

void DataShow::append(DeviceData devicedata)
{
    if(!label.contains("DV")){
        this->pulseRate->setNum(devicedata.pulseRate);
        this->Spo2->setNum(devicedata.spo2Sat);
    }
    //chart->setSerie(index++,devicedata);
    devicechart->setSerie(index++,devicedata);
}

void DataShow::GetFeature_clicked()
{
    //r.push_back(this->Patientlabel,this->chart,c->lastfilenum);
    r.push_back(this->Patientlabel,this->devicechart,c->lastfilenum);
}

void DataShow::SendCommand_clicked(){
    socket->write("S VIEW AFE ON\r\n");
}
