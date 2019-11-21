#ifndef HEARTCHART_H
#define HEARTCHART_H
#include "verticalline.h"
#include <QtCharts>
#include <QtCharts/QChart>
#include <vector>
#include <iostream>
#include <algorithm>
#include <QChartView>
QT_CHARTS_USE_NAMESPACE;
#include <QLineSeries>
#include <QValueAxis>
#include <QString>
#include <QSplineSeries>
#include <QPointF>
#include <QVector>
#include <vector>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QBluetoothAddress>
#include <QSizePolicy>
#include "dataclass.h"

#ifdef ENABLD_HEARTCHART
class HeartChart
{
public:
    HeartChart(int axisXMax=500,int axisYMax1=25,int axisYMax2=100,QString label="");
    bool addLine(QLineSeries* series);
    QLineSeries* initSerie(int position);
    unsigned int GetaxisXMax();
    bool setLabel(QString label);
    bool setSerie(int x,DeviceData data);
    bool setRange(int min,int max);
    void autorange();
    void setSeries(QList<double> list);
    ~HeartChart();
public:

    QChartView* AreachartView;
    QChart* Areachart;
    QChartView* BarchartView;
    QChart* Barchart;
    QChartView* PulsechartView;
    QChart* Pulsechart;
    QChartView* LinechartView;
    QChart* Linechart;

    QList<QChartView*> chartList;
    QLineSeries *serieszero;
    QLineSeries *series;
    QLineSeries *pulseseseries;
    QLineSeries *lineseries;
    QAreaSeries *areaseries;
    QBarSeries  *barseries;
    QBarSet *set;

    QValueAxis *AreaaxisX;
    QValueAxis *AreaaxisY;

    QValueAxis *BaraxisX;
    QValueAxis *BaraxisY;

    QValueAxis *PulseaxisX;
    QValueAxis *PulseaxisY;

    QValueAxis *LineaxisX;
    QValueAxis *LineaxisY;

    VerticalLine *verticalline;

    std::vector<int> barpoint;
    std::vector<int> pulsepoint;
    std::vector<int> areapoint;
    std::vector<double> linepoint;

    bool ifbar = false;
    bool ifarea = false;
    bool ifpulse = false;
    bool ifline = false;

    QList<int> axisYmin = {0,0,0};
    QList<int> axisYmax = {0,0,0};

    unsigned int axisXMax;
    unsigned int axisYMax;
    QString label;

};
#endif
#endif // HEARTCHART_H
