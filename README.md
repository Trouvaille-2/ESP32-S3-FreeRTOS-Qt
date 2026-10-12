## 📺 演示视频 (Video Demo)

本项目完整实现了软件在环仿真 (SIL) 与真实伺服电机硬件闭环控制。

- 🔗 **B站高清视频演示**：[点击前往观看 (60fps)](【花了两天时间用qt为我的单片机项目做了个适配上位机】 https://www.bilibili.com/video/BV1DRpR6YEQg/?share_source=copy_web&vd_source=2d98364ddbde0ca7ad44d9d15b291ace)
- 📱 **移动端扫码观看**：

<p align="left">
  <img src="docs/bilibili_qr.png" width="160" alt="B站演示二维码" />
</p>

> **视频核心工况展示**：
> - **阶段 1：单速度闭环 (0~20s)**：200 RPM 阶跃响应，稳态无静差巡航与平滑刹停；
> - **阶段 2：位置-速度双环定位 (20~50s)**：1320 脉冲（单圈）高精度伺服定位，无超调平稳刹车进站，稳态误差 $\le 0.4\%$。
