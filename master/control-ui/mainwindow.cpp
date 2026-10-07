#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("直流伺服双环测控系统");
    resize(1200,750);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSerial(QSerialPort *serial)
{
    m_serial = serial;//接管串口指针

    //提示状态栏
    if(m_serial && m_serial ->isOpen())
    {
         ui->statusbar->showMessage(QString("✅ 已连接设备: %1").arg(m_serial->portName()));
    }
}

//标准crc16校验算法
uint16_t MainWindow::calculateCRC(const QByteArray &data)
{
    uint16_t crc = 0xFFFF;
        for (int i = 0; i < data.size(); ++i) {
            crc ^= (uint8_t)data.at(i);
            for (int j = 0; j < 8; ++j) {
                if (crc & 0x0001) {
                    crc = (crc >> 1) ^ 0xA001;
                } else {
                    crc = crc >> 1;
                }
            }
        }
        return crc;
}

//Modbus 0x06功能码：向单片机写单个寄存器
void MainWindow::writeRegister(uint16_t reg, uint16_t val)
{
    if (!m_serial || !m_serial->isOpen()) {
            ui->statusbar->showMessage("⚠️ 串口未连接，发送失败！");
            return;
        }

    QByteArray frame;
    frame.append((char)0x01);  //1.从机地址（0x01）
    frame.append((char)0x06);   //2.功能码0x06
    frame.append((char)((reg>>8)&0xFF));//3.寄存器地址高字节
    frame.append((char)(reg&0xff));//4.寄存器地址低字节
    frame.append((char)((val>>8)&0xFF));//5.写入数值高字节
    frame.append((char)(reg&0xFF));//6.写入数值低字节

    //7.计算并追加CRC16
    uint16_t crc=calculateCRC(frame);
    frame.append((char)(crc&0xFF));
    frame.append((char)((crc>>8)&0xFF));

    //8.真正通过物理串口发射出去
    m_serial->write(frame);
}

/*按钮点击事件*/

//点击[使能运行]
void MainWindow::on_btnEnable_clicked()
{
    writeRegister(REG_CTRL_CMD,1);//把0x0000寄存器改成1
    ui->statusbar->showMessage("🟢 指令已发送：使能运行");

}

//点击[正常停止]
void MainWindow::on_btnStop_clicked()
{
    writeRegister(REG_CTRL_CMD, 0); // 把 0x0000 寄存器改成 0
        ui->statusbar->showMessage("🟡 指令已发送：正常停止");
}

// 点击【紧急刹车】
void MainWindow::on_btnBrake_clicked()
{
    writeRegister(REG_CTRL_CMD, 2); // 把 0x0000 寄存器改成 2
    ui->statusbar->showMessage("🔴 指令已发送：紧急短路刹车");
}
// 点击【编码器回零】
void MainWindow::on_btnResetPos_clicked()
{
    writeRegister(REG_CTRL_CMD, 3); // 把 0x0000 寄存器改成 3
    ui->statusbar->showMessage("🔄 指令已发送：编码器位置清零");
}
// 切换【模式下拉框】
void MainWindow::on_comboMode_currentIndexChanged(int index)
{
    writeRegister(REG_CTRL_MODE, index); // 把 0x0001 寄存器改成选中的索引 (0/1/2)
    ui->statusbar->showMessage(QString("⚙️ 模式已切换为：%1").arg(ui->comboMode->currentText()));
}

