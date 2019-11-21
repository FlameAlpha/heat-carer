#ifndef CHARTSERIES_H
#define CHARTSERIES_H

#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <QDebug>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>

QT_CHARTS_USE_NAMESPACE;

namespace ChartSeries {

    class AreaSeries
    {
    public:
        AreaSeries(int n_):n(n_){
            seriesup = new QLineSeries();
            serieszero = new QLineSeries();
            for(unsigned int i=0;i<n;i++){
                   points.push_back(5);
                   seriesup->append(i,5);
                   serieszero->append(i,0);
            }
            series = new QAreaSeries(seriesup,serieszero);
            series->setUseOpenGL(true);//openGl
            serieszero->setUseOpenGL(true);//openGl
                seriesup->setUseOpenGL(true);//openGl
        }
        operator  QAbstractSeries * () { return series; }
        void replace(int x,double data){
            seriesup->replace(x,x,data);
            points[x]=data;
        }
        void SetSeries(QList<double> list)
        {
            points.clear();
            seriesup->clear();
            int i = 0;
            for(auto item:list){
                seriesup->append(i++,item);
                points.push_back(item);
            }
        }
        std::pair<double,double> MaxMin(){
            return std::pair<double,double>(*std::max_element(points.begin(),points.end()),*std::min_element(points.begin(),points.end()));
        }
        unsigned long long length(){
            return points.size();
        }
        ~AreaSeries(){
            delete seriesup;
            delete serieszero;
            delete series;
        }
        int n;
        bool autoRangeSupport = true;
        QLineSeries *seriesup;
        QLineSeries *serieszero;
        QAreaSeries *series;
        std::vector<double> points;
    };

    class BarSeries
    {
    public:
        BarSeries(int n_){
            Q_UNUSED(n_)
            set = new QBarSet("set");
            series = new QBarSeries();
            set->append(0);
            set->append(60);
            set->append(0);
            //points.push_back(0);
            //points.push_back(60);
            series->append(set);
            series->setUseOpenGL(true);//openGl
        }
        operator  QAbstractSeries*() { return series; }
        void replace(int x,double data){
            Q_UNUSED(x)
            set->replace(1,data);
            //points[1] = data;
        }
        void SetSeries(QList<double> list)
        {
            for(auto item:list){
                set->replace(1,item);
            }
        }
        std::pair<double,double> MaxMin(){
            return std::pair<double,double>(100,10);
        }
        unsigned long long length(){
            return 2;
        }
        ~BarSeries(){
            delete series;
            delete set;
        }
        bool autoRangeSupport = false;
        QBarSeries  *series;
        QBarSet *set;
        std::vector<double> points;
    };
    class LineSeries
    {
    public:
        LineSeries(int n_):n(n_){
            //qDebug() <<" Start LineSeries Init";
            series = new QLineSeries();
            for(unsigned int i=0;i<n;i++){
                   series->append(i,5);
                   points.push_back(5);
            }
            series->setUseOpenGL(true);//openGl
            //qDebug() <<" LineSeries Init Over";
        }
        operator QAbstractSeries*() { return series; }
        //QAbstractSeries* getSeries() { return series; }
        void replace(int x,double data){
            series->replace(x,x,data);
            points[x]=data;
        }
        void SetSeries(QList<double> list)
        {
            points.clear();
            series->clear();
            int i = 0;
            for(auto item:list){
                series->append(i++,item);
                points.push_back(item);
            }
        }
        std::pair<double,double> MaxMin(){
            return std::pair<double,double>(*std::max_element(points.begin(),points.end()),*std::min_element(points.begin(),points.end()));
        }
        unsigned long long length(){
            return points.size();
        }
        ~LineSeries(){
            delete series;
        }
        int n;
        bool autoRangeSupport = true;
        QLineSeries *series;
        std::vector<double> points;
    };

    class SplineSeries
    {
    public:
        SplineSeries(int n_):n(n_){
            series = new QSplineSeries();
            for(unsigned int i=0;i<n;i++){
                   series->append(i,5);
                   points.push_back(5);
            }
            series->setUseOpenGL(true);//openGl
        }
        operator  QAbstractSeries*() { return series; }
        void replace(int x,double data){
            series->replace(x,x,data);
            points[x]=data;
        }
        void SetSeries(QList<double> list)
        {
            points.clear();
            series->clear();
            int i = 0;
            for(auto item:list){
                series->append(i++,item);
                points.push_back(item);
            }
        }
        std::pair<double,double> MaxMin(){
            return std::pair<double,double>(*std::max_element(points.begin(),points.end()),*std::min_element(points.begin(),points.end()));
        }
        unsigned long long length(){
            return points.size();
        }
        ~SplineSeries(){
            delete series;
        }
        int n;
        bool autoRangeSupport = true;
        QSplineSeries *series;
        std::vector<double> points;
    };

    class ScatterSeries
    {
    public:
        ScatterSeries(int n_):n(n_){
            series = new QScatterSeries();
            for(unsigned int i=0;i<n;i++){
                   series->append(i,5);
                   points.push_back(5);
            }
            series->setUseOpenGL(true);//openGl
        }
        operator  QAbstractSeries*() { return series; }
        void replace(int x,double data){
            series->replace(x,x,data);
            points[x]=data;
        }
        void SetSeries(QList<double> list)
        {
            points.clear();
            series->clear();
            int i = 0;
            for(auto item:list){
                series->append(i++,item);
                points.push_back(item);
            }
        }
        std::pair<double,double> MaxMin(){
            return std::pair<double,double>(*std::max_element(points.begin(),points.end()),*std::min_element(points.begin(),points.end()));
        }
        unsigned long long length(){
            return points.size();
        }
        ~ScatterSeries(){
            delete series;
        }
        int n;
        bool autoRangeSupport = true;
        QScatterSeries *series;
        std::vector<double> points;
    };
}
#endif // CHARTSERIES_H
