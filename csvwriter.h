#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <thread>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "dataclass.h"
#include <sys/timeb.h>
#include <QList>
#include <mutex>

#define maxdatalength 256*60*5

class CsvWriter
{
public:
    long long filenum;
    long long lastfilenum;
    int count;
    bool havewrite = false;
    std::ofstream log;
    bool already = true;
    CsvWriter();
    void InsertDataMultiThread(QList<DeviceData> datalist);
    void InsertData(QList<DeviceData> datalist);
};

#endif // CSVWRITER_H
