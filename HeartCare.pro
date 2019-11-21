#-------------------------------------------------
#
# Project created by Flame 2019-09-04
# lastest version 3.0 , released by Flame on 4 Sep 2019
# functional class:
# * bluetooth devices show
# * bluetooth socket <multithread>
# * chart series template
# * chart show view reedit
# * handle sql <mysql> <influxdb ><multithread>
# * verticalline
# * csv writer
# * AI report by used python API <multithread>
#-------------------------------------------------

QT       += core gui
QT       += bluetooth
QT       += sql
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HeartCare
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17 # c++ version
CONFIG += console # show with console or not
DEFINES += EMBED_PYTHON # used embed python or not
# DEFINES += ENABLD_HEARTCHART # used heart chart or not
SOURCES += \
        main.cpp \
        datashow.cpp \
    socketdataprocess.cpp \
    bluetoothservice.cpp \
    heartchart.cpp \
    verticalline.cpp \
    keyin.cpp \
    report.cpp \
    csvwriter.cpp \
    chartshow.cpp \
    chartseries.cpp \
    devicecharts.cpp \
    waitingwindows.cpp \
    InfluxDB/src/HTTP.cxx \
    InfluxDB/src/InfluxDB.cxx \
    InfluxDB/src/InfluxDBFactory.cxx \
    InfluxDB/src/Point.cxx \
    InfluxDB/src/UDP.cxx \
    InfluxDB/src/UnixSocket.cxx \
    handleinfluxdb.cpp \
    handlesql.cpp

HEADERS += \
        datashow.h \
    socketdataprocess.h \
    bluetoothservice.h \
    heartchart.h \
    verticalline.h \
    keyin.h \
    dataclass.h \
    report.h \
    csvwriter.h \
    chartshow.h \
    chartseries.h \
    devicecharts.h \
    waitingwindows.h \
    InfluxDB/include/InfluxDB.h \
    InfluxDB/include/InfluxDBFactory.h \
    InfluxDB/include/Point.h \
    InfluxDB/include/Transport.h \
    InfluxDB/src/HTTP.h \
    InfluxDB/src/UDP.h \
    InfluxDB/src/UnixSocket.h \
    InfluxDB/src/UriParser.h \
    handlesql.h \
    handleinfluxdb.h

FORMS += \
        datashow.ui \
    bluetoothservice.ui \
    keyin.ui \
    waitingwindows.ui

RC_ICONS += heart_128.ico
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$PWD/Python37/libs/ -lpython37

INCLUDEPATH += $$PWD/Python37/libs
DEPENDPATH += $$PWD/Python37/libs

win32: LIBS += -L$$PWD/curl/lib/ -llibcurl

INCLUDEPATH += $$PWD/curl
DEPENDPATH += $$PWD/curl
