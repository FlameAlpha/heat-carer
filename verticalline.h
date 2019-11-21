/**
* @file     imuverticalline.h
* @class    ImuVerticalLine
* @brief    this file includes class for painting vertical lines which position data is from markers in imudata.
* @details  when you doubleclick this line,it will be deleted by you.
* @author   Flame
* @date     03.05.2019
*/

#ifndef IMUVERTICALLINE_H
#define IMUVERTICALLINE_H

#include <iostream>
#include <QChart>


#include <QPointF>
#include <QString>
#include <QLineSeries>
#include <QValueAxis>
#include <QDebug>
QT_CHARTS_USE_NAMESPACE;

class VerticalLine : public QLineSeries
{
    Q_OBJECT
public:
    VerticalLine(int position, QValueAxis *axisY, Qt::GlobalColor color=Qt::red, int PenSize=10);
    VerticalLine(int position, double high, Qt::GlobalColor color=Qt::red, int PenSize=10);
    void changeHigh(int position,QValueAxis *axisY);
    void changeHigh(int position,double data);
signals:
    void deleteme(int position);

public slots:
    void sendposition(const QPointF &point);
    //void tooltip(QPointF point, bool state);
private:
    int position;
};

#endif // IMUVERTICALLINE_H
