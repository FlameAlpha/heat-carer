#ifndef WAITINGWINDOWS_H
#define WAITINGWINDOWS_H

#include <QWidget>
#include <QPainter>
#include <QCloseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>
namespace Ui {
class WaitingWindows;
}

class WaitingWindows : public QWidget
{
    Q_OBJECT

public:
    explicit WaitingWindows(QWidget *parent = nullptr,QWidget * showWindows_=nullptr);
    ~WaitingWindows();
    void closeEvent(QCloseEvent *event);
private:
    QWidget * showWindows;
    Ui::WaitingWindows *ui;
};

#endif // WAITINGWINDOWS_H
