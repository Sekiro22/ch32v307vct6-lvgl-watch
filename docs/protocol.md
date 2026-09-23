# 接口与数据协议

当前工程没有应用层串口、网络或自定义帧协议。本文件记录已经实现的板级通信接口。

## GC9A01 SPI 接口

### 引脚

| 信号 | 引脚 | 方向 | 功能 |
| --- | --- | --- | --- |
| SCK | PA5 | MCU 输出 | SPI1 时钟 |
| MOSI | PA7 | MCU 输出 | SPI1 数据 |
| CS | PA4 | MCU 输出 | 低电平选中屏幕 |
| DC | PA3 | MCU 输出 | 低电平命令，高电平数据 |
| RST | PA2 | MCU 输出 | 低电平硬件复位 |
| BL | 3.3 V | 电源 | 背光常亮，不受 MCU 控制 |

### SPI 参数

- 主机模式；
- 单线发送，不使用 MISO；
- 8 bit 数据；
- MSB first；
- CPOL=0、CPHA=0（SPI Mode 0）；
- SPI1 分频为 8；系统与 APB2 当前配置下约为 18 MHz；
- 软件控制 CS。

### 命令/数据时序

命令发送：

```text
CS=0 -> DC=0 -> 发送 1 字节命令 -> 等待 SPI 空闲 -> CS=1
```

数据发送：

```text
CS=0 -> DC=1 -> 发送 N 字节数据 -> 等待 SPI 空闲 -> CS=1
```

像素连续写入时，CS 在整批像素发送期间保持低电平。RGB565 每个像素先发送高字节，再发送低字节。

### 关键 GC9A01 命令

| 命令 | 用途 | 当前值/行为 |
| --- | --- | --- |
| `0x11` | Sleep Out | 发送后延时 120 ms |
| `0x36` | MADCTL | `0x48`，用于当前安装方向和颜色顺序 |
| `0x3A` | Pixel Format | `0x55`，RGB565 |
| `0x21` | Display Inversion On | 当前启用 |
| `0x29` | Display On | 初始化末尾发送 |
| `0x2A` | Column Address Set | 设置 X 起止坐标 |
| `0x2B` | Row Address Set | 设置 Y 起止坐标 |
| `0x2C` | Memory Write | 开始写入像素 |

## LVGL 刷新接口

`lvgl_flush()` 接收包含首尾坐标的 `lv_area_t` 和 RGB565 像素指针。宽高按包含端点计算：

```text
width  = x2 - x1 + 1
height = y2 - y1 + 1
count  = width * height
```

发送完成后必须调用 `lv_display_flush_ready(display)`。当前 SPI 为阻塞方式，因此该调用发生在物理发送完成之后。

## 调试串口

| 参数 | 值 |
| --- | --- |
| 外设 | USART1 |
| TX | PA9 |
| 波特率 | 115200 |

启动时输出系统时钟、芯片 ID、FreeRTOS 版本，UI 任务启动时输出 `UI_Thread start!`。当前未定义串口接收协议。

## 外部 Flash 规划

W25Q128 尚未接入代码。后续建议使用独立 SPI2：PB13/SCK、PB14/MISO、PB15/MOSI、PB12/CS。该分配目前只是规划，实施前必须检查与新增外设的引脚冲突。
