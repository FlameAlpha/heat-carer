#include "datashow.h"
#include "report.h"
#include "bluetoothservice.h"
#include <QApplication>
#include "handlesql.h"
#include <QByteArray>
#include <QDebug>
#include <mutex>
#include "chartshow.h"
#include "waitingwindows.h"
#include <thread>
#include "handleinfluxdb.h"
HandleInfluxDB infux;
Report r;
HandleSQL m;
std::mutex mt;
bool initreport = false;
bool initmysql= false;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.addLibraryPath(a.applicationDirPath()+"/plugins");
    SetConsoleTitleA("HeartCare Program");
    QApplication::setStyle("fusion"); //"windows", "windowsvista", "fusion", or "macintosh"
    BlueToothService d;
    WaitingWindows wait(nullptr,&d);
    wait.show();
    d.show();
    d.setEnabled(false);
    wait.setWindowTitle("Waiting for init !");
    r.init_self(&wait);
    m.init_self(&wait);
    return a.exec();
}
