#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>

#include <QtCharts>
QT_CHARTS_USE_NAMESPACE

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
    void sendReadRequest();//定时器每50ms触发一次，发送03读指令
    void onSerialReadyRead();//串口有新数据到达时触发

private:
    Ui::MainWindow *ui;
    QSerialPort *m_serial;//保存传过来的串口指针

    QTimer *m_pollTimer;//50ms轮询定时器
    QByteArray m_rxBuffer;//接收数据缓冲区

    //示波器核心对象
    QChart *m_chart;  //示波器大画布
    QChartView *m_chartView; //承载画布的视图控件
    QLineSeries *m_seriesActualPos;// 实际位置曲线 (蓝色)
    QLineSeries *m_seriesActualSpd;// 实际转速曲线 (绿色)
    QValueAxis *m_axisX;           // 横轴：时间轴 (秒)
    QValueAxis *m_axisYPos;        // 左纵轴：位置轴 (脉冲)

    double m_timeCounter;    //记录当前运行时间

    void initChart();   //示波器初始化函数

    //辅助工具函数
    uint16_t calculateCRC(const QByteArray &data);
    void writeRegister(uint16_t reg,uint16_t val);//发送Modbus 0x06写寄存器

    // 常用寄存器地址宏（和单片机完全一致）
    static const uint16_t REG_CTRL_CMD  = 0x0000; // 控制字: 0-停止, 1-使能, 2-刹车, 3-回零
    static const uint16_t REG_CTRL_MODE = 0x0001; // 模式: 0-开环, 1-单速度, 2-双环

};

#endif // MAINWINDOW_H
