#include "keyin.h"
#include "ui_keyin.h"

KeyIn::KeyIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyIn)
{
    ui->setupUi(this);
}

KeyIn::~KeyIn()
{
    delete ui;
}

void KeyIn::on_CancelpushButton_clicked()
{
    QMessageBox::information(this,tr("Info"),tr("Please input information!"));
}

void KeyIn::on_OkpushButton_clicked()
{
    if(ui->lineEdit->text().size()>=6){
        deviceinfo.patientID = ui->lineEdit->text();
        deviceinfo.type = ui->comboBox->currentIndex()+1;
        if(!m.InsertPatientDeviceInfo(deviceinfo)){
            QMessageBox::information(this,tr("Info"),tr("Please input right information!"));
        }
        else{
            this->close();
            emit(isObtain(deviceinfo));
        }
    }
    else {
         QMessageBox::information(this,tr("Info"),tr("Please input right information!"));
    }
}
