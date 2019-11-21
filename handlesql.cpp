#include "handlesql.h"

extern std::mutex mt;
extern bool initreport;
extern bool initmysql;
HandleSQL::HandleSQL(QObject *parent,QString Databasename_,QString HostName_,int Port_,QString UserName_,QString Password_) :
    QObject(parent)
{   
    Databasename = Databasename_;
    HostName = HostName_;
    Port = Port_;
    UserName = UserName_;
    Password = Password_;
}
void HandleSQL::init(){
    db = QSqlDatabase::addDatabase(SQLString);
    db.setHostName(HostName);//Ip
    db.setPort(Port);
    db.setDatabaseName(Databasename);
    db.setUserName(UserName);
    db.setPassword(Password);
    if (!db.open()){
        qDebug()<<"can not open";
        return;
    }
    std::thread t(&HandleSQL::InsertDataProcessing,this);
    t.detach();
    if(initreport && wait!=nullptr)
        wait->close();
    initmysql = true;
}
void HandleSQL::init_self(QWidget * wait_){
    std::thread t(&HandleSQL::init,this);
    t.detach();
    wait = wait_;
}
bool HandleSQL::CreatTable(QString name){
    mt.lock();
    already = false;
    mt.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //qDebug()<<"Start Creat Table "+name.toLower()+" !";
    QSqlQuery query(db);
    if(!name.contains("DV")){
        QString command = "CREATE TABLE IF NOT EXISTS `hospital`.`"+name.toLower()+"` (`NO.` INT NOT NULL AUTO_INCREMENT,`ID` VARCHAR(45) NOT NULL DEFAULT \"00:00:00:00:00:00\",`DATETIME` INT(64) NULL DEFAULT NULL,`CURRENTTIME` DATETIME NULL DEFAULT CURRENT_TIMESTAMP,`SIGNAL` INT NULL DEFAULT NULL,`PULSERATE` INT NULL DEFAULT NULL,`SPO2` INT NULL DEFAULT NULL,`BAR` INT NULL DEFAULT NULL,`PLETH` INT NULL DEFAULT NULL,`spare1` VARCHAR(45) NULL DEFAULT NULL,`spare2` VARCHAR(45) NULL DEFAULT NULL,`spare3` VARCHAR(45) NULL DEFAULT NULL,`spare4` VARCHAR(45) NULL DEFAULT NULL,`spare5` VARCHAR(45) NULL DEFAULT NULL,PRIMARY KEY (`NO.`));";
        //while(!query.prepare(command)) { query.clear(); std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
        if(query.exec(command)){
            mt.lock();
            already = true;
            mt.unlock();
            return true;
        }
    }
    else {
        QString command = "CREATE TABLE IF NOT EXISTS `hospital`.`"+name.toLower()+"` (`NO.` INT NOT NULL AUTO_INCREMENT,`ID` VARCHAR(45) NOT NULL DEFAULT \"00:00:00:00:00:00\",`DATETIME` INT(64) NULL DEFAULT NULL,`CURRENTTIME` DATETIME NULL DEFAULT CURRENT_TIMESTAMP,`ECG` INT NULL DEFAULT NULL,`ACCX` INT NULL DEFAULT NULL,`ACCY` INT NULL DEFAULT NULL,`ACCZ` INT NULL DEFAULT NULL,`HR` INT NULL DEFAULT NULL,`spare1` VARCHAR(45) NULL DEFAULT NULL,`spare2` VARCHAR(45) NULL DEFAULT NULL,`spare3` VARCHAR(45) NULL DEFAULT NULL,`spare4` VARCHAR(45) NULL DEFAULT NULL,`spare5` VARCHAR(45) NULL DEFAULT NULL,PRIMARY KEY (`NO.`));";
        //while(!query.prepare(command)) { query.clear(); std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
        if(query.exec(command)){
            mt.lock();
            already = true;
            mt.unlock();
            return true;
        }
    }
    mt.lock();
    already = true;
    mt.unlock();
    return false;
}

void HandleSQL::InsertDataMultiThread(QString name,QList<DeviceData> alldata){
    if(lastdata.time >= alldata[0].time)
    {
        alldata[0].time = lastdata.time + 1;
    }
    lastdata.time = alldata[0].time + alldata.size() - 1;
    QSqlQuery query(db);
    QString command ;
    if(!name.contains("DV")){
        command = "INSERT INTO `hospital`.`"+name.toLower()+"` (`ID`, `DATETIME`, `SIGNAL`, `PULSERATE`, `SPO2`, `BAR`, `PLETH`) ";
        command  = command + "VALUES " ;
        int i = 0;
        for(auto data:alldata){
            command  = command +
                    "('" + data.address.toString() + "'," +\
                      QString::number(int(alldata[0].time+(++i)*5)) + "," +\
                      QString::number(int(data.signal)) + "," +\
                      QString::number(int(data.pulseRate)) + "," +\
                      QString::number(int(data.spo2Sat)) + "," +\
                      QString::number(int(data.bar)) + "," +\
                      QString::number(int(data.pleth)) + ")";
            if(i == alldata.size())
                command = command + ";";
            else {
                command = command + ",";
            }
        }
    }
    else{
        command = "INSERT INTO `hospital`.`"+name.toLower()+"` (`ID`, `DATETIME`, `ECG`, `ACCX`, `ACCY`, `ACCZ`, `HR`) ";
        command  = command + "VALUES " ;
        int i = 0;
        for(auto data:alldata){
            command  = command +
                    "('" + data.address.toString() + "'," +\
                      QString::number(int(alldata[0].time+(++i)*3.91)) + "," +\
                      QString::number(int(data.ECG)) + "," +\
                      QString::number(int(data.Accx)) + "," +\
                      QString::number(int(data.Accy)) + "," +\
                      QString::number(int(data.Accz)) + "," +\
                      QString::number(int(data.heartRate)) + ")";
            if(i == alldata.size())
                command = command + ";";
            else {
                command = command + ",";
            }
        }
    }

    mt.lock();
    commandlist.push_back(command);
    mt.unlock();
}

bool HandleSQL::InsertDataProcessing(){
    QSqlQuery query(db);
    while(1){
        if(!commandlist.isEmpty()&&already){
            query.exec(commandlist.first());
            mt.lock();
            commandlist.pop_front();
            mt.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return true;
}

bool HandleSQL::InsertData(QString name,QList<DeviceData> alldata){
    //InsertDataMultiThread();
    std::thread t(&HandleSQL::InsertDataMultiThread,this,name,alldata);
    t.detach();
    return true;
}

bool HandleSQL::isObtainPatientDeviceInfo(QString address){
    mt.lock();
    already = false;
    mt.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    QSqlQuery query(db);
    QString command = "SELECT * FROM `hospital`.`devices` WHERE `ID`= '"+address+"'";
    //while(!query.prepare(command)) {query.clear(); std::this_thread::sleep_for(std::chrono::milliseconds(100));}
    if(!query.exec(command)){
        mt.lock();
        already = true;
        mt.unlock();
        qDebug()<<"select error !";
        return false;
    }
    if(query.size()==0){
        mt.lock();
        already = true;
        mt.unlock();
        qDebug()<<"no member !";
        return false;
    }
    else {
        mt.lock();
        already = true;
        mt.unlock();
        qDebug()<<"select right !";
        return true;
    }

}

DeviceInfo HandleSQL::GetPatientDeviceInfo(QString address){
    mt.lock();
    already = false;
    mt.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    QSqlQuery query(db);
    DeviceInfo info;
    QString command = "SELECT `ID`, `NAME`, `TYPE`, `PATIENTID` FROM `hospital`.`devices` WHERE `ID`= '"+address+"'";
    //while(!query.prepare(command)) { query.clear(); std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
    query.exec(command);
    if(query.first()) {
        info.address = query.value(0).toString();
        info.name = query.value(1).toString();
        info.type = query.value(2).toInt();
        info.patientID = query.value(3).toString();
    }
    mt.lock();
    already = true;
    mt.unlock();
    return info;
}

bool HandleSQL::InsertPatientDeviceInfo(DeviceInfo info){
    mt.lock();
    already = false;
    mt.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    QSqlQuery query(db);
    QString command = "INSERT INTO `hospital`.`devices` (`ID`, `NAME`, `TYPE`, `PATIENTID`) "
                      "VALUES ('"+info.address+"','"+info.name+"',"+QString::number(info.type)+",'"+info.patientID+"')";
    //while(!query.prepare(command)) { query.clear(); std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
    if(!query.exec(command)){
        qDebug()<<"insert error!";
        qDebug()<<command;
        mt.lock();
        already = true;
        mt.unlock();
        return false;
    }
    else{
        qDebug()<<"insert right!";
        mt.lock();
        already = true;
        mt.unlock();
        return true;
    }
}

HandleSQL::~HandleSQL()
{
    db.close();
}
