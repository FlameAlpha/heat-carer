#include "csvwriter.h"
extern std::mutex mt;
CsvWriter::CsvWriter()
{
    count = 0;
    filenum = 0;
    lastfilenum = 0;
}

void CsvWriter::InsertDataMultiThread(QList<DeviceData> datalist)
{
    while(!datalist.isEmpty()){
        if(!havewrite){
            timeb current;
            ftime(&current);
            lastfilenum = filenum;
            filenum = current.time*1000+current.millitm;
            log = std::ofstream(("./DataLog/"+QString::number(filenum)+".csv").toStdString().data(), std::ios::out | std::ios::trunc);
            havewrite = true;
        }
        auto data = datalist.first();
        datalist.pop_front();
        timeb current;
        ftime(&current);
        log << current.time*1000+current.millitm <<","<<data.ECG<<std::endl;
        count++;
        if(count > maxdatalength){
            log.close();
            havewrite = false;
            count = 0;
        }
    }
}
void CsvWriter::InsertData(QList<DeviceData> datalist)
{
    std::thread t(&CsvWriter::InsertDataMultiThread,this,datalist);
    t.detach();
}
