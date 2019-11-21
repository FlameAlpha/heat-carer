#ifndef REPORT_H
#define REPORT_H
#define PY_SSIZE_T_CLEAN
#include <Python37/include/Python.h>
#include <QList>
#include <QString>
#include <QDebug>
#include <thread>
#include "heartchart.h"
#include <QLabel>
#include "devicecharts.h"
//#include <mutex>
//#define FilenameisString "filename"
#pragma execution_character_set("utf-8")
extern std::mutex mt;
template<typename FileNameType,typename ChartType>
class reportShow
{
public:
    QList<FileNameType> filenameList;
    QList<QLabel *> labelList;
    QList<ChartType *> chartList;

    unsigned int n=0;
    reportShow() {}
    void push_back(QLabel *label,ChartType * chart,FileNameType filenum){
        labelList.push_back(label);
        chartList.push_back(chart);
        filenameList.push_back(filenum);
        n++;
    }
    QLabel * getCurrentLabel(){
        return labelList.front();
    }
    ChartType * getCurrentChart()
    {
        return chartList.front();
    }
    FileNameType getCurrentFilename()
    {
        return filenameList.front();
    }
    void removeFrontItem()
    {
        if(n>0){
            labelList.pop_front();
            chartList.pop_front();
            filenameList.pop_front();
            n--;
        }
    }
    bool isEmpty()
    {
        return n <= 0;
    }
};

class Report
{
public:
    Report();
    ~Report();
    void init();
    void init_self(QWidget * wait_ = nullptr);
    bool Process();
    void push_back(QLabel * label,DeviceChart * chart,long long int filenum);
    void ProcessMultiThread();
    QLabel * label;
    QWidget * wait;
    DeviceChart * chart;
    long long int filenum;
    reportShow<long long int,DeviceChart> show;
    QList<QString> reportstr = {QString::fromUtf8("������λ����"),QString::fromUtf8("������λ�����ں�"),QString::fromUtf8("δ֪����"),QString::fromUtf8("����������/״̬�쳣")};
    bool isAlready = true;
    PyObject * pModule = nullptr;
    PyObject * pFunc = nullptr;
};

#endif // REPORT_H
