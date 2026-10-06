#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    m_serial = new QSerialPort(this);
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

    //实现实时查询串口功能
    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&Dialog::scanAvailablePorts);
    m_timer->start(1000);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::scanAvailablePorts()
{
    //获取当前系统可用串口列表
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();

    QStringList currentPortNames;
    for(int i=0;i<portList.size();++i)
    {
        currentPortNames<<portList.at(i).portName();
    }

    //判断是否和上次串口一样
    if(currentPortNames == m_lastPortNames)
    {
        return;
    }

    m_lastPortNames = currentPortNames;
    ui->comboBaud->setCurrentText("9600"); // 自动重置回默认的 9600

    //刷新下拉列表
    ui->comboPort->clear();

    if(portList.isEmpty())
    {
        ui->lblStatus->setText("未检测到任何可用串口，请插入设备！");
        return;
    }else{
    //遍历串口
        for(int i=0;i<portList.size();++i)
        {
        const QSerialPortInfo &info = portList.at(i);
        ui->comboPort->addItem(info.portName() + " (" + info.description() + ")", info.portName());
        }
    }

    ui->lblStatus->setText(QString("✅ 已检测到 %1 个可用串口").arg(portList.size()));

    //----------高亮闪烁提示动画----------//
    ui->lblStatus->setStyleSheet("background-color:#fff3cd;color:#856404;font-weight:bold;border:1px solid #ffeeba;border-radius:4px;");

    QTimer::singleShot(600,this,[=]()
    {
        ui->lblStatus->setStyleSheet("");
    });

}

void Dialog::on_btnRefresh_clicked()
{
    m_lastPortNames.clear();
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

    if(m_serial->isOpen())
    {
        m_serial->close();
    }

    m_serial->setPortName(portName);
    m_serial->setBaudRate(baudRate);//波特率
    m_serial->setDataBits(QSerialPort::Data8);//数据位
    m_serial->setParity(QSerialPort::NoParity);//奇偶校验位
    m_serial->setStopBits(QSerialPort::OneStop);//停止位

    if(!m_serial->open(QIODevice::ReadWrite))
    {
        QMessageBox::warning(this,"连接失败","无法打开该串口，可能已被其他软件占用！");
        return;
    }

    accept();
}

void Dialog::on_btnclear_clicked()
{
    reject();//关闭对话框
}
