#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //增加串口接管函数
    void initSerial(QSerialPort *serial);


private slots:
    // 4 个控制按钮的点击事件槽函数
    void on_btnEnable_clicked();   // 使能运行
    void on_btnStop_clicked();     // 正常停止
    void on_btnBrake_clicked();    // 紧急刹车
    void on_btnResetPos_clicked(); // 编码器回零
    void on_comboMode_currentIndexChanged(int index); // 控制模式切换

private:
    Ui::MainWindow *ui;
    QSerialPort *m_serial;//保存传过来的串口指针

    //辅助工具函数
    uint16_t calculateCRC(const QByteArray &data);
    void writeRegister(uint16_t reg,uint16_t val);//发送Modbus 0x06写寄存器

    // 常用寄存器地址宏（和单片机完全一致）
    static const uint16_t REG_CTRL_CMD  = 0x0000; // 控制字: 0-停止, 1-使能, 2-刹车, 3-回零
    static const uint16_t REG_CTRL_MODE = 0x0001; // 模式: 0-开环, 1-单速度, 2-双环

};

#endif // MAINWINDOW_H
