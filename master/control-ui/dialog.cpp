#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPortInfo>
#include <QSerialPort>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //设置标题
    setWindowTitle("连接伺服设备");

    //设置波特率下拉框
    ui->comboBaud->addItem("9600");
    ui->comboBaud->addItem("115200");
    ui->comboBaud->addItem("19200");
    ui->comboBaud->addItem("38400");
    ui->comboBaud->setCurrentText("9600");

    //窗口打开，自动执行扫描串口
    scanAvailablePorts();

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::scanAvailablePorts()
{
    ui->comboPort->clear();

    //获取当前系统可用串口列表
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();

    if(portList.isEmpty())
    {
        ui->lblStatus->setText("未检测到任何可用串口，请插入设备！");
        return;
    }

    //遍历串口
    for(int i=0;i<portList.size();++i)
    {
        const QSerialPortInfo &info = portList.at(i);
        ui->comboPort->addItem(info.portName() + " (" + info.description() + ")", info.portName());
    }

    ui->lblStatus->setText(QString("✅ 已检测到 %1 个可用串口").arg(portList.size()));
}

void Dialog::on_btnRefresh_clicked()
{
    scanAvailablePorts();
}

void Dialog::on_btnConnect_clicked()
{
    //获取当前选中的串口
    QString portName = ui->comboPort->currentData().toString();

    if(portName.isEmpty())
    {
        QMessageBox::warning(this,"警告","请先选择一个串口");
        return;
    }

    //获取当前选中的波特率
    int baudRate=ui->comboBaud->currentText().toInt();

    //配置串口参数
}

void Dialog::on_btnclear_clicked()
{
    reject();//关闭对话框
}
