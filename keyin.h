#ifndef KEYIN_H
#define KEYIN_H

#include <QDialog>
#include <QMessageBox>
#include "handlesql.h"

extern HandleSQL m;
namespace Ui {
class KeyIn;
}

class KeyIn : public QDialog
{
    Q_OBJECT

public:
    DeviceInfo deviceinfo;
    explicit KeyIn(QWidget *parent = nullptr);
    ~KeyIn();

private slots:
    void on_CancelpushButton_clicked();
    void on_OkpushButton_clicked();
signals:
    void isObtain(DeviceInfo deviceinfo);
private:
    Ui::KeyIn *ui;
};

#endif // KEYIN_H
