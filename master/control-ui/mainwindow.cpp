#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("直流伺服双环测控系统");
    resize(1200,750);

    initChart();
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

         //1.当串口接收引脚有新数据进来时，立刻通知onSerialReadyRead处理
         connect(m_serial,&QSerialPort::readyRead,this,&MainWindow::onSerialReadyRead);

         //2.启动50ms定时器，每50毫秒自动问单片机要一次数据
         m_pollTimer = new QTimer(this);
         connect(m_pollTimer,&QTimer::timeout,this,&MainWindow::sendReadRequest);
         m_pollTimer->start(50);//50ms
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

//定时向单片机发问：读取0x0004~0x000B(共8个寄存器：目标，实际位置，转速，PWM，误差)
void MainWindow::sendReadRequest()
{
    if(!m_serial || !m_serial->isOpen())
    {
        return;
    }

    QByteArray frame;
    frame.append((char)0x01); // 1. 从机地址 (0x01)
        frame.append((char)0x03); // 2. 功能码 0x03 (读保持寄存器)
        frame.append((char)0x00); // 3. 起始地址高字节 (0x00)
        frame.append((char)0x04); // 4. 起始地址低字节 (从 0x0004 开始读)
        frame.append((char)0x00); // 5. 寄存器数量高字节 (0x00)
        frame.append((char)0x08); // 6. 寄存器数量低字节 (连续读 8 个寄存器)

        // 计算并追加 CRC16
         uint16_t crc = calculateCRC(frame);
        frame.append((char)(crc & 0xFF));
        frame.append((char)((crc >> 8) & 0xFF));
        // 发送出这一问！
        m_serial->write(frame);
}


//串口收到单片机的回复数据：解析Modbus报文并刷新左侧LCD屏
void MainWindow::onSerialReadyRead()
{
    //1.把串口硬件缓冲区里收到的字节全拿出来，存进我们的接收蓄水池
    m_rxBuffer.append(m_serial->readAll());

    //一帧标准的8寄存器回复是21字节（1从机+1功能码+1字节数16+16字节数据+2字节crc）
    const int FRAME_LEN=21;

    while(m_rxBuffer.size()>=FRAME_LEN)
    {
        //帧头校验：如果第1字节不是0x01或者第2字节不是0x03，说明是乱码杂音，滑动丢弃1字节
        if((uint8_t)m_rxBuffer.at(0)!=0x01 || (uint8_t)m_rxBuffer.at(1)!=0x03)
        {
            m_rxBuffer.remove(0,1);
            continue;
        }

        //取出整帧数据
        QByteArray frame = m_rxBuffer.left(FRAME_LEN);

        //校验CRC16
        uint16_t calcCrc = calculateCRC(frame.left(FRAME_LEN - 2));
        uint16_t recvCrc = (uint8_t)frame.at(FRAME_LEN - 2) | ((uint8_t)frame.at(FRAME_LEN - 1) << 8);

        if(calcCrc ==recvCrc)
        {
            //校验完全正确，开始解剖里面的真实物理数据：

            // 提取实际位置脉冲 (32 位整型，占第 7,8,9,10 字节)
            uint16_t pos_h = ((uint8_t)frame.at(7) << 8) | (uint8_t)frame.at(8);
            uint16_t pos_l = ((uint8_t)frame.at(9) << 8) | (uint8_t)frame.at(10);
            int32_t actual_pos = (int32_t)(((uint32_t)pos_h << 16) | pos_l);

            // ② 提取实际转速 (RPM，第 13,14 字节)
            int16_t actual_spd = (int16_t)(((uint8_t)frame.at(13) << 8) | (uint8_t)frame.at(14));
            // ③ 提取 PWM 占空比 (千分比，第 15,16 字节，除以 10 变回百分比)
            int16_t pwm_raw = (int16_t)(((uint8_t)frame.at(15) << 8) | (uint8_t)frame.at(16));
            float pwm_duty = (float)pwm_raw / 10.0f;
            // ④ 提取跟踪误差 (脉冲数，第 17,18 字节)
            int16_t pos_error = (int16_t)(((uint8_t)frame.at(17) << 8) | (uint8_t)frame.at(18));
            // 🎯 将最新鲜的数字实时更新到 UI 界面上的 4 个 LCD 屏幕！
            ui->lblActualPos->display(actual_pos);
            ui->lblActualSpd->display(actual_spd);
            ui->lblPwmOutput->display(QString::number(pwm_duty, 'f', 1));
            ui->lblPosErrot->display(pos_error); // (注意 ui 中控件名是 lblPosErrot)

            // ================= 📈 示波器动态波形推进 =================
            m_timeCounter += 0.05; // 轮询周期是 50ms (0.05秒)
            // 1. 给两条曲线追加最新鲜的数据点 (时间点, 实时数值)
            m_seriesActualPos->append(m_timeCounter, actual_pos);
            m_seriesActualSpd->append(m_timeCounter, actual_spd);
            // 2. 心电图自动向右滚动：当时间超过 10 秒后，X 轴窗口跟随当前时间向右平移！
            if (m_timeCounter > 10.0) {
                m_axisX->setRange(m_timeCounter - 10.0, m_timeCounter);
            }
            // 3. 内存保护：当点数超过 600 个（过去 30 秒的数据）时，自动删掉最老的一个点
            if (m_seriesActualPos->count() > 600) {
                m_seriesActualPos->remove(0);
                m_seriesActualSpd->remove(0);
            }

            // 实时把反馈值打在目标框下方的 feedback_num 标签上
            if (ui->comboMode->currentIndex() == 2) {
                ui->feedback_num->setText(QString("%1 脉冲").arg(actual_pos));
            } else {
                ui->feedback_num->setText(QString("%1 RPM").arg(actual_spd));
            }

            // 从缓冲区移除已处理的这 21 字节
            m_rxBuffer.remove(0, FRAME_LEN);
        } else {
            // CRC错误，丢掉第 1 字节寻找下一个可能帧
            m_rxBuffer.remove(0, 1);
        }
    }
}

void MainWindow::initChart()
{
    m_timeCounter = 0.0;

    // 1. 创建两条动态折线
    m_seriesActualPos = new QLineSeries();
    m_seriesActualPos->setName("实际位置 (脉冲)");
    m_seriesActualPos->setPen(QPen(QColor(0, 122, 255), 2)); // 科技蓝，线宽 2

    m_seriesActualSpd = new QLineSeries();
    m_seriesActualSpd->setName("实际转速 (RPM)");
    m_seriesActualSpd->setPen(QPen(QColor(46, 204, 113), 2)); // 荧光绿，线宽 2

    // 2. 创建图表画布并添加曲线
    m_chart = new QChart();
    m_chart->addSeries(m_seriesActualPos);
    m_chart->addSeries(m_seriesActualSpd);
    m_chart->setTitle("伺服电机实时运动动态波形");

    // 3. 配置坐标轴
    // X 轴：时间轴 (默认显示过去 10 秒)
    m_axisX = new QValueAxis();
    m_axisX->setTitleText("时间 (秒)");
    m_axisX->setRange(0, 10);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_seriesActualPos->attachAxis(m_axisX);
    m_seriesActualSpd->attachAxis(m_axisX);
    // Y 轴：数值轴 (默认范围 -500 ~ +500)
    m_axisYPos = new QValueAxis();
    m_axisYPos->setTitleText("幅值");
    m_axisYPos->setRange(-500, 500);
    m_chart->addAxis(m_axisYPos, Qt::AlignLeft);
    m_seriesActualPos->attachAxis(m_axisYPos);
    m_seriesActualSpd->attachAxis(m_axisYPos);
    // 4. 创建视图控件，并开启抗锯齿（让曲线极度丝滑平整）
    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    // 5. 把做好的示波器画布，装进右侧 3/4 的 widgetChart 容器里！
    QVBoxLayout *layout = new QVBoxLayout(ui->widgetChart);
    layout->addWidget(m_chartView);
    layout->setContentsMargins(0, 0, 0, 0); // 边距贴紧


}

// 当用户在设定值输入框输入完毕按回车（或鼠标点到别处）时，自动下发给单片机
void MainWindow::on_setting_num_editingFinished()
{
    int val = ui->setting_num->value();
    int mode = ui->comboMode->currentIndex(); // 0-开环, 1-单速度, 2-双环

    if (mode == 0) {
        // 开环模式：下发 PWM 油门 (乘以 10 变成千分比)
        writeRegister(0x000A, (uint16_t)(val * 10));
        ui->statusbar->showMessage(QString("🎯 目标已下发：开环占空比 %1 %").arg(val));
    }
    else if (mode == 1) {
        // 单速度模式：下发目标转速 (RPM)
        writeRegister(REG_TARGET_SPD, (uint16_t)val);
        ui->statusbar->showMessage(QString("🎯 目标已下发：速度 %1 RPM").arg(val));
    }
    else if (mode == 2) {
        // 位置双环模式：下发 32 位目标脉冲（拆成高 16 位和低 16 位发送）
        writeRegister(0x0004, (uint16_t)((val >> 16) & 0xFFFF)); // REG_TARGET_POS_H
        writeRegister(0x0005, (uint16_t)(val & 0xFFFF));         // REG_TARGET_POS_L
        ui->statusbar->showMessage(QString("🎯 目标已下发：位置 %1 脉冲").arg(val));
    }
}
