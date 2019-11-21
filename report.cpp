#include "report.h"
extern bool initreport;
extern bool initmysql;
Report::Report(){
}


Report::~Report()
{
    Py_DECREF(pModule);
    Py_DECREF(pFunc);
    Py_Finalize();

}
void Report::init_self(QWidget * wait_){
    std::thread t(&Report::init,this);
    t.detach();
    wait = wait_;
}
void Report::init()
{
    if (!Py_IsInitialized()){
        //Py_Finalize();
        Py_Initialize();
    }
    if (!Py_IsInitialized()){
        qDebug()<<"init error";
        return ;
    }
#ifndef EMBED_PYTHON
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.clear()");
    PyRun_SimpleString("sys.path.append(os.path.abspath('./'))");
    PyRun_SimpleString("sys.path.append(os.path.abspath('./PythonLibs'))");
#endif
    pModule = PyImport_ImportModule("report");
    if (pModule == nullptr)
    {
        qDebug() << "Don't find Module !" ;
        return ;
    }
    pFunc = PyObject_GetAttrString(pModule, "report");
    if (pFunc == nullptr)
    {
        qDebug() << "Don't find Function !";
        return ;
    }
    std::thread tp(&Report::ProcessMultiThread,this);
    tp.detach();
    if(initmysql && wait != nullptr)
        wait->close();
    initreport = true;
}

void Report::push_back(QLabel * label,DeviceChart * chart,long long int filenum_)
{
    mt.lock();
    show.push_back(label,chart,filenum_);
    mt.unlock();
}

bool Report::Process()
{
    label->setText(QString::fromUtf8("²¡ÈË×´Ì¬¼à²âÖÐ"));
    QList<double> FeatureLine;
    QList<QString> TypeList;
#ifndef FilenameisString
    PyObject* pRet = PyObject_CallFunction(pFunc,"ss","./DataLog/",(QString::number(filenum)+".csv").toStdString().data());
#else
    PyObject* pRet = PyObject_CallFunction(pFunc,"ss","./DataLog/",(filenum+".csv").toStdString().data());
#endif
    PyObject* pTypeList = PyList_GetItem(pRet,0);
    PyObject* pFeatureLine = PyList_GetItem(pRet,1);
    for(int i=0;i<PyList_Size(pTypeList);i++){
        TypeList.push_back(reportstr[PyLong_AsLong(PyList_GetItem(pTypeList,i))]);
    }
    for(int i=0;i<PyList_Size(pFeatureLine);i++){
        FeatureLine.push_back(PyFloat_AsDouble(PyList_GetItem(pFeatureLine,i)));
    }
    qDebug() << "typeSize : "<<PyList_Size(pTypeList)<<" type : " << TypeList;
    qDebug() << "lineSize : "<<PyList_Size(pFeatureLine)<<" line : " << FeatureLine;
    chart->setSeries(FeatureLine);
    QString temp = QString::fromUtf8("²¡ÈË×´Ì¬:");
    for(auto item:TypeList)
    {
        temp =temp +" "+item;
    }
    label->setText(temp);
    Py_DECREF(pRet);
    Py_DECREF(pTypeList);
    Py_DECREF(pFeatureLine);
    isAlready = true;
    return true;
}


void Report::ProcessMultiThread()
{
    while(1){
        if(!show.isEmpty()&&isAlready)
        {
            isAlready = false;
            label = show.getCurrentLabel();
            chart = show.getCurrentChart();
            filenum = show.getCurrentFilename();
            std::thread t(&Report::Process,this);
            t.detach();
            mt.lock();
            show.removeFrontItem();
            mt.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
