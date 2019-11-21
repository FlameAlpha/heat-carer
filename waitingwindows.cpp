#include "waitingwindows.h"
#include "ui_waitingwindows.h"

WaitingWindows::WaitingWindows(QWidget *parent, QWidget * showWindows_) :
    QWidget(parent),
    showWindows(showWindows_),
    ui(new Ui::WaitingWindows)
{
    ui->setupUi(this);
    //this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint| this->windowFlags() | Qt::WindowStaysOnTopHint);
}
void WaitingWindows::closeEvent(QCloseEvent *event){
    event->accept();
    showWindows->setEnabled(true);
}
WaitingWindows::~WaitingWindows()
{
    delete ui;
}
