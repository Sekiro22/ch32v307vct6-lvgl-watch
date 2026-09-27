# CH32V307VCT6 LVGL Watch

基于 CH32V307VCT6、FreeRTOS、LVGL 9.6 和 GC9A01 圆形 TFT 的嵌入式界面工程。

当前版本已经完成：

- CH32V307VCT6 基础启动与 FreeRTOS 调度；
- GC9A01 240×240 RGB565 显示驱动；
- SPI1 DMA 局部刷新和 LVGL 9.6 显示任务；
- LVGL Pro Editor 导出的圆形表盘；
- PE1～PE6 按键扫描、20 ms 周期消抖和按下事件队列；
- RTC 计数驱动时、分、秒 Line 指针，每秒刷新表盘。

## 硬件

| 模块信号 | MCU 引脚 | 说明 |
| --- | --- | --- |
| GC9A01 SCL/SCK | PA5 | SPI1 SCK |
| GC9A01 SDA/MOSI | PA7 | SPI1 MOSI |
| GC9A01 CS | PA4 | 软件片选 |
| GC9A01 DC | PA3 | 命令/数据选择 |
| GC9A01 RST | PA2 | 硬件复位 |
| GC9A01 BL | 3.3 V | 背光常亮 |
| 调试串口 TX | PA9 | USART1，115200 baud |
| 按键 1～6 | PE1～PE6 | 内部上拉，按下为低电平 |

显示器虽然是圆形面板，但控制器坐标和 LVGL 分辨率仍为 240×240。重要控件建议放在中央约 170×170 的安全区域内。

## 软件结构

```text
Dev/                    GC9A01 驱动及按键扫描
FreeRTOS/               FreeRTOS 内核及 RISC-V 移植
Ld/                     当前使用的链接脚本
Middlewares/LVGL/       LVGL 9.6 源码
Middlewares/lv_conf.h   LVGL 配置
SRC/                    WCH Core、外设库和调试支持
User/app/               UI、按键事件和 RTC 表针逻辑
User/ui/                LVGL Pro 导出的 C 代码
User/main.c             系统入口
docs/                   架构、接口、任务和调试文档
```

## 构建

推荐使用 MounRiver Studio 2：

1. 导入仓库根目录中的既有工程；
2. 选择 `obj` 活动配置；
3. 执行 Clean，再执行 Build；
4. 烧录生成的 `obj/FreeRTOS.hex`。

命令行构建要求 MounRiver 的 `riscv-none-embed-*` 工具位于 `PATH`。`obj/` 是 IDE 生成目录，不纳入 Git。

最近一次已验证构建：

```text
text    236896
data       384
bss      53136
```

以上为 2026-09-27 构建结果。`text + data` 约 237280/262144 字节，理论 Flash 余量约 24 KB；新增完整中文字库或大图片前必须重新评估空间。大资源计划存放到外部 W25Q128，但该模块尚未接入。

## UI 更新流程

LVGL Pro Editor 项目当前以 9.5 格式导出，表盘代码已在本工程 LVGL 9.6 上通过编译。

1. 在 Editor 中修改 XML；
2. 按 `Ctrl+E` 仅导出 C 代码；
3. 将生成的 C/H 文件同步到 `User/ui/`；
4. 不直接修改名称包含 `_gen` 的文件；
5. Clean 并重新构建固件。

更多信息见 [架构](docs/architecture.md)、[接口](docs/protocol.md)、[任务](docs/tasks.md) 和 [调试记录](docs/debugging.md)。

## 当前限制

- 按键任务目前只打印按键名称，尚未接入菜单导航；
- RTC 当前使用内部 LSI（预分频 39999），首次时间写在代码中，尚无校时入口。LSI 精度需实测校准，完全断电后的持续走时未验证；
- 生成 UI 的矩形指针保持零旋转，运行时由非生成代码中的 LVGL Line 绘制动态指针；
- LVGL 堆为 32 KB，新增复杂控件后需重新测量内存峰值。

## 下一阶段

1. 在 LVGL Pro 设计一级菜单，并将按键事件接入 UI 任务内的菜单导航；
2. 增加校时入口，验证 RTC 复位保持、LSI 误差和表针长期运行；
3. 接入 W25Q128 与 LVGL 文件系统，存放后续字体和图片资源。
