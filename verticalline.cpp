#include "verticalline.h"

VerticalLine::VerticalLine(int position, QValueAxis *axisY, Qt::GlobalColor color,int PenSize)
{

    this->position = position;
    setPen(QPen(color, PenSize, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin));
    append(position,axisY->max());
    append(position,axisY->min());
    setUseOpenGL(true);//openGl 加速
    //connect(this, &QLineSeries::hovered, this, &ImuVerticalLine::tooltip);
    //connect(this, &QLineSeries::doubleClicked, this, &VerticalLine::sendposition);
}
VerticalLine::VerticalLine(int position, double high, Qt::GlobalColor color,int PenSize)
{

    this->position = position;
    setPen(QPen(color, PenSize, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin));
    append(position,high);
    append(position,0);
    setUseOpenGL(true);//openGl 加速
    //connect(this, &QLineSeries::hovered, this, &ImuVerticalLine::tooltip);
    //connect(this, &QLineSeries::doubleClicked, this, &VerticalLine::sendposition);
}
void VerticalLine::sendposition(const QPointF &point)
{
    Q_UNUSED(point)
    emit(VerticalLine::deleteme(position));
    this->disconnect(SIGNAL(deleteme(int)));
}
void VerticalLine::changeHigh(int position,QValueAxis *axisY)
{
    this->clear();
    append(position,axisY->max());
    append(position,axisY->min());
}
void VerticalLine::changeHigh(int position,double data)
{
    this->replace(1,position,data);
}
