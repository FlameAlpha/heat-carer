#include "heartchart.h"
#ifdef ENABLD_HEARTCHART
HeartChart::HeartChart(int axisXMax, int axisYMax1, int axisYMax2, QString label):
    AreaaxisX(new QValueAxis),
    AreaaxisY(new QValueAxis),
    AreachartView(new QChartView),
    Areachart(new QChart),
    BaraxisX(new QValueAxis),
    BaraxisY(new QValueAxis),
    BarchartView(new QChartView),
    Barchart(new QChart),
    PulseaxisX(new QValueAxis),
    PulseaxisY(new QValueAxis),
    PulsechartView(new QChartView),
    Pulsechart(new QChart),
    LineaxisX(new QValueAxis),
    LineaxisY(new QValueAxis),
    LinechartView(new QChartView),
    Linechart(new QChart)
{
    qDebug()<<"chart init ! ";
    chartList.push_back(AreachartView);
    chartList.push_back(BarchartView);
    chartList.push_back(PulsechartView);
    chartList.push_back(LinechartView);

    series = new QLineSeries();
    serieszero = new QLineSeries();

    barseries = new QBarSeries();
    set = new QBarSet("PluseRate");
    pulseseseries = new QLineSeries();
    lineseries = new QLineSeries();

    this->axisXMax = axisXMax;

    AreaaxisX->setRange(0,axisXMax);
    AreaaxisX->setLabelFormat("%g");
//    AreaaxisX->setTitleText("axisX");

    BaraxisX->setRange(0,2);
    BaraxisX->setLabelFormat("%g");
//    BaraxisX->setTitleText("axisX");

    PulseaxisX->setRange(0,axisXMax);
    PulseaxisX->setLabelFormat("%g");
//    PulseaxisX->setTitleText("axisX");

    LineaxisX->setRange(0,20);
    LineaxisX->setLabelFormat("%g");

    this->axisYMax = axisYMax1;
    AreaaxisY->setRange(0,axisYMax2);
//    AreaaxisY->setTitleText("axisY");

    BaraxisY->setRange(0,axisYMax1);
//    BaraxisY->setTitleText("axisY");

    PulseaxisY->setRange(0,axisYMax2);
    LineaxisY->setRange(0,100);
    //set->setBorderColor(Qt::blue);
    //set->setColor(Qt::blue);

    set->append(0);
    set->append(60);
    barseries->append(set);

    pulseseseries->setColor(Qt::red);
    lineseries->setColor(Qt::red);

    for(unsigned int i=0;i<axisXMax;i++){
       series->append(i,5);
       serieszero->append(i,0);
       pulseseseries->append(i,5);
       barpoint.push_back(5);
       areapoint.push_back(0);
       pulsepoint.push_back(5);
    }
    areaseries = new QAreaSeries(series,serieszero);
    //qDebug()<<"areaseries = new QAreaSeries(serieszero, series);";
    Areachart->addSeries(areaseries);
    //qDebug()<<"Areachart->addSeries(areaseries);";
    Barchart->addSeries(barseries);
    //qDebug()<<"Barchart->addSeries(barseries);";
    Pulsechart->addSeries(pulseseseries);
    Linechart->addSeries(lineseries);

    barseries->setUseOpenGL(true);//openGl
    pulseseseries->setUseOpenGL(true);//openGl
    lineseries->setUseOpenGL(true);//openGl
    series->setUseOpenGL(true);//openGl

    Areachart->setAxisX(AreaaxisX,areaseries);
    Areachart->setAxisY(AreaaxisY,areaseries);
    Areachart->legend()->hide();
    Areachart->axisX()->hide();

    Barchart->setAxisX(BaraxisX,barseries);
    Barchart->setAxisY(BaraxisY,barseries);
    Barchart->legend()->hide();
    Barchart->axisX()->hide();

    Pulsechart->setAxisX(PulseaxisX,pulseseseries);
    Pulsechart->setAxisY(PulseaxisY,pulseseseries);
    Pulsechart->legend()->hide();
    Pulsechart->axisX()->hide();

    Linechart->setAxisX(LineaxisX,lineseries);
    Linechart->setAxisY(LineaxisY,lineseries);
    Linechart->legend()->hide();
    Linechart->axisX()->hide();

    this->label = label;
    Areachart->setTitle(this->label+"脉搏波形 Pulse Waveform");
    Barchart->setTitle(this->label+"脉相 Pulse State");
    Pulsechart->setTitle(this->label+"心率 Heart Rate");
    Linechart->setTitle(this->label+"特征曲线 Feature Line");
    QFont serifFont("Times", 15, QFont::Bold);
    Areachart->setTitleFont(serifFont);
    Barchart->setTitleFont(serifFont);
    Pulsechart->setTitleFont(serifFont);
    Linechart->setTitleFont(serifFont);

    PulsechartView->setRubberBand(QChartView::RectangleRubberBand);
    PulsechartView->setChart(Pulsechart);

    LinechartView->setRubberBand(QChartView::RectangleRubberBand);
    LinechartView->setChart(Linechart);

    AreachartView->setRubberBand(QChartView::RectangleRubberBand);
    AreachartView->setChart(Areachart);

    BarchartView->setRubberBand(QChartView::RectangleRubberBand);
    BarchartView->setChart(Barchart);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSizePolicy sizePolicyFixed(QSizePolicy::Fixed, QSizePolicy::Expanding);

    AreachartView->setMinimumSize(500,300);
    PulsechartView->setMinimumSize(500,300);
    BarchartView->setMinimumSize(100,200);
    LinechartView->setMinimumSize(500,400);

    AreachartView->chart()->setTheme(QChart::ChartThemeBlueIcy);
    PulsechartView->chart()->setTheme(QChart::ChartThemeBlueIcy);
    LinechartView->chart()->setTheme(QChart::ChartThemeBlueIcy);
    BarchartView->chart()->setTheme(QChart::ChartThemeBlueIcy);

    BarchartView->setMaximumSize(16777215,16777215);
    AreachartView->setMaximumSize(16777215,16777215);
    PulsechartView->setMaximumSize(16777215,16777215);
    LinechartView->setMaximumSize(16777215,16777215);

    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicyFixed.setHorizontalStretch(0);
    sizePolicyFixed.setVerticalStretch(0);
    sizePolicyFixed.setHeightForWidth(BarchartView->sizePolicy().hasHeightForWidth());
    BarchartView->setSizePolicy(sizePolicyFixed);
    sizePolicy.setHeightForWidth(AreachartView->sizePolicy().hasHeightForWidth());
    AreachartView ->setSizePolicy(sizePolicy);
    sizePolicy.setHeightForWidth(PulsechartView->sizePolicy().hasHeightForWidth());
    PulsechartView->setSizePolicy(sizePolicy);
    sizePolicy.setHeightForWidth(LinechartView->sizePolicy().hasHeightForWidth());
    LinechartView->setSizePolicy(sizePolicy);

    verticalline = new VerticalLine(0,PulseaxisY);
    verticalline->setColor(QColor(250,250,250));
    Pulsechart->addSeries(verticalline);
    Pulsechart->setAxisX(PulseaxisX,verticalline);
    Pulsechart->setAxisY(PulseaxisY,verticalline);
    verticalline->setVisible(true);
}

QLineSeries* HeartChart::initSerie(int position)
{
    QLineSeries *serietemp = new QLineSeries;
    serietemp->setColor(QColor(1,0,0));
    serietemp->append(position,AreaaxisY->max());
    serietemp->append(position,AreaaxisY->min());
    serietemp->setUseOpenGL(true);//openGl 加速
    Areachart->addSeries(serietemp);
    Areachart->setAxisX(AreaaxisX,serietemp);
    Areachart->setAxisY(AreaaxisY,serietemp);
    return serietemp;
}

bool HeartChart::setSerie(int x, DeviceData data)
{
    if(data.ifbar){
        set->replace(1,data.bar);
        barpoint[x]=data.bar;
        ifbar = true;
    }
    if(data.ifpleth){
        series->replace(x,x,data.pleth);
        areapoint[x]=data.pleth;
        ifarea = true;
    }
    if(data.ifECG){
        pulseseseries->replace(x,x,data.ECG);
        pulsepoint[x]=data.ECG;
        verticalline->changeHigh(x,PulseaxisY);
        ifpulse = true;
    }
//    if(x<axisXMax-1){
//        if(data.ifECG)
//            pulseseseries->replace(x+1,x+1,0);
//        if(data.ifpleth)
//            series->replace(x+1,x+1,0);
//    }
//    if(x<axisXMax-2){
//        if(data.ifECG)
//            pulseseseries->replace(x+2,x+2,0);
//        if(data.ifpleth)
//            series->replace(x+2,x+2,0);
//    }
    return true;
}
bool HeartChart::addLine(QLineSeries *serie)
{
    Areachart->addSeries(serie);
    return true;
}
void HeartChart::setSeries(QList<double> list)
{
    lineseries->clear();
    linepoint.clear();
    int i = 0;
    for(auto item:list){
        lineseries->append(i++,item);
        linepoint.push_back(item);
    }
    LineaxisY->setMax(*std::max_element(linepoint.begin(),linepoint.end())+10);
    LineaxisY->setMin(*std::min_element(linepoint.begin(),linepoint.end())-10);
    LineaxisX->setMax(linepoint.size()-1);
}
void HeartChart::autorange()
{
    if(ifpulse){
        PulseaxisY->setMax(*std::max_element(pulsepoint.begin(),pulsepoint.end())+20);
        PulseaxisY->setMin(*std::min_element(pulsepoint.begin(),pulsepoint.end())-20);
        ifpulse = false;
    }
    if(ifbar){
        BaraxisY->setMax(*std::max_element(barpoint.begin(),barpoint.end())+10);
        BaraxisY->setMin(0);//*std::min_element(barpoint.begin(),barpoint.end())
        ifbar = false;
    }
    if(ifarea){
        AreaaxisY->setMax(*std::max_element(pulsepoint.begin(),pulsepoint.end())+20);
        AreaaxisY->setMin(*std::min_element(pulsepoint.begin(),pulsepoint.end())-20);
        ifarea = false;
    }
}
unsigned int HeartChart::GetaxisXMax()
{
    return axisXMax;
}

bool HeartChart::setLabel(QString label)
{
    this->label = label;
    return true;
}

HeartChart::~HeartChart()
{
    delete AreachartView;
    delete Areachart;
    delete BarchartView;
    delete Barchart;
    delete PulsechartView;
    delete Pulsechart;

    delete serieszero;
    delete series;
    delete pulseseseries;
    delete areaseries;
    delete barseries;
    delete set;

    delete AreaaxisX;
    delete AreaaxisY;

    delete BaraxisX;
    delete BaraxisY;

    delete PulseaxisY;
    delete PulseaxisX;

    delete verticalline;
}
#endif
