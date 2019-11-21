#ifndef DEVICECHARTS_H
#define DEVICECHARTS_H
#include "chartshow.h"
#include "chartseries.h"
enum Devices
{
      Default, DV, BerryMed
};

class DeviceChart
{
    Devices device;
public:
    ChartShow<ChartSeries::LineSeries,int> * HeartRateChart;
    ChartShow<ChartSeries::LineSeries,int> * ReportChart;
    ChartShow<ChartSeries::LineSeries,int> * PulseLineChart;
    ChartShow<ChartSeries::BarSeries,int,false,2,0,true,25,0,false> * PulseBarChart;
    ChartShow<ChartSeries::AreaSeries,int> * PulseAreaChart;
    DeviceChart(Devices device_){
        qDebug()<<"Start Device Init ! ";
        device = device_;
        if(device==DV){
            qDebug()<<"New DV Device! ";
            HeartRateChart = new ChartShow<ChartSeries::LineSeries,int>("心率 Heart Rate");
            ReportChart = new ChartShow<ChartSeries::LineSeries,int>("特征曲线 Feature Line");
        }
        else if(device==BerryMed){
            qDebug()<<"New BerryMed Device! ";
            PulseLineChart = new ChartShow<ChartSeries::LineSeries,int>("心率 Heart Rate");
            PulseBarChart = new ChartShow<ChartSeries::BarSeries,int,false,2,0,true,25,0,false>("脉率 Pulse State",windowsSize<int>(300,100));
            PulseAreaChart = new ChartShow<ChartSeries::AreaSeries,int>("脉相 Pulse Waveform");
        }
        qDebug()<<"Device Init Over ! ";
    }
    void setSerie(int index,DeviceData devicedata){
        if(device==DV){
            HeartRateChart->setSerie(index,devicedata.ECG);
        }
        else if(device==BerryMed){
            PulseLineChart->setSerie(index,devicedata.pleth);
            PulseBarChart->setSerie(index,devicedata.bar);
            PulseAreaChart->setSerie(index,devicedata.pleth);
        }
    }
    void setSeries(QList<double> list){
        ReportChart->SetSeries(list);
    }
    void autorange(){
        if(device==DV){
            HeartRateChart->autorange();
        }
        else if(device==BerryMed){
            PulseLineChart->autorange();
            //PulseBarChart->autorange();
            PulseAreaChart->autorange();
        }
    }
    ~DeviceChart(){
        if(device==DV){
            delete HeartRateChart;
            delete ReportChart;
        }
        else if(device==BerryMed){
            delete PulseLineChart;
            delete PulseBarChart;
            delete PulseAreaChart;
        }
    }
};

#endif // DEVICECHARTS_H
