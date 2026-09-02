#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPort/>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_btnRefresh_clicked();//刷新按钮
    void on_btnConnect_clicked();//连接按钮
    void on_btnclear_clicked();

private:
    Ui::Dialog *ui;
    void scanAvailablePorts();//扫描电脑可用串口
};

#endif // DIALOG_H
