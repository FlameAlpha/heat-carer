#ifndef CHARTSHOW_H
#define CHARTSHOW_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <QDebug>
#include <QChartView>
#include <dataclass.h>
#include "chartseries.h"
#include "verticalline.h"
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE;
template<typename Type>
class windowsSize{
public:
    windowsSize(Type Height,Type Width):
        Height(Height),
        Width(Width)
    {

    }
    Type Height;
    Type Width;
};

template<typename serieType,typename dataType,bool setXRange=true,int axisXMax=500,int axisXMin=0,bool setYRange=true,int axisYMax=100,int axisYMin=0,bool ifshelter=true>
class ChartShow
{
public:
    QString label;
    QChart * chart;
    serieType * series;
    QValueAxis * axisX;
    QValueAxis * axisY;
    QChartView * chartView;
    std::vector<int> point;
    VerticalLine * verticalline;
    ChartShow(QString label_,windowsSize<int> size=windowsSize<int>(300,500)):
        chart(new QChart),
        axisX(new QValueAxis),
        axisY(new QValueAxis),
        chartView(new QChartView),
        label(label_)
    {
        qDebug() << label_ <<" Start Chart Init";
        if(setXRange)
            axisX->setRange(axisXMin,axisXMax);
        axisX->setLabelFormat("%g");
    //  axisX->setTitleText("axisX");
        if(setYRange)
            axisY->setRange(axisYMin,axisYMax);
    //  axisY->setTitleText("axisY");
        series = new serieType(axisXMax-axisXMin);
        chart->addSeries(series->series);
        chart->setAxisX(axisX,series->series);
        chart->setAxisY(axisY,series->series);
        chart->legend()->hide();
        chart->axisX()->hide();
        chart->setTitle(label_);
        QFont serifFont("Helvetica", 15, QFont::Bold);
        //chart->setTitleFont(serifFont);
        chartView->setRubberBand(QChartView::RectangleRubberBand);
        chartView->setChart(chart);
        chartView->setBaseSize(size.Width,size.Height);
        //chartView->setMinimumSize(size.Width,size.Height);
        //chartView->setMaximumSize(16777215,16777215);
        chartView->chart()->setTheme(QChart::ChartThemeBlueIcy);
        chartView->chart()->setTitleFont(serifFont);
        chartView->setFont(serifFont);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        //sizePolicy.setHeightForWidth(true);
        chartView->setSizePolicy(sizePolicy);
        if(ifshelter){
            verticalline = new VerticalLine(0,axisY);
            verticalline->setColor(QColor(250,250,250));
            chart->addSeries(verticalline);
            chart->setAxisX(axisX,verticalline);
            chart->setAxisY(axisY,verticalline);
            verticalline->setVisible(true);
        }
        qDebug() << label_ << "Chart Init Over";
    }
    void setLabel(QString label_){
        label = label_;
    }
    void setSerie(int x, dataType data){
        //qDebug() << x%axisXMax <<' '<<label+" replace";
        series->replace(x%axisXMax,data);
        if(ifshelter){
            //qDebug() << "verticalline move";
            verticalline->changeHigh(x%axisXMax,axisY);
        }
        if((x % int(axisXMax/10) == 0) && series->autoRangeSupport){
            //qDebug() << "autorange";
            autorange();
        }
    }
    void SetSeries(QList<double> list){
        series->SetSeries(list);
        if(series->autoRangeSupport){
            //qDebug() << "autorange";
            autorange();
        }
    }
    void setAxisYRange(int min,int max){
        axisY->setMax(max);
        axisY->setMin(min);
    }
    void autorange(){
        auto maxmin = series->MaxMin();
        axisY->setMax(maxmin.first + 10);
        axisY->setMin(maxmin.second - 10);
        axisX->setMin(0);
        axisX->setMax(series->length()-1);
    }
    operator QWidget * (){return chartView;}
    ~ChartShow(){
        delete chart;
        delete series;
        delete axisX;
        delete axisY;
        delete chartView;
        if(ifshelter)
            delete verticalline;
    }
};

#endif // CHARTSHOW_H
