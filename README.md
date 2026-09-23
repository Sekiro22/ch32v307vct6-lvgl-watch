# CH32V307VCT6 LVGL Watch

基于 CH32V307VCT6、FreeRTOS、LVGL 9.6 和 GC9A01 圆形 TFT 的嵌入式界面工程。

当前版本已经完成：

- CH32V307VCT6 基础启动与 FreeRTOS 调度；
- GC9A01 240×240 RGB565 显示驱动；
- LVGL 9.6 显示端口和周期处理任务；
- LVGL Pro Editor 导出界面的最小接入；
- 主界面显示 `LVGL UI` 标签和 `OK` 按钮。

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

显示器虽然是圆形面板，但控制器坐标和 LVGL 分辨率仍为 240×240。重要控件建议放在中央约 170×170 的安全区域内。

## 软件结构

```text
Dev/                    GC9A01 驱动
FreeRTOS/               FreeRTOS 内核及 RISC-V 移植
Ld/                     当前使用的链接脚本
Middlewares/LVGL/       LVGL 9.6 源码
Middlewares/lv_conf.h   LVGL 配置
SRC/                    WCH Core、外设库和调试支持
User/app/               应用任务
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
text    221128
data       336
bss      36716
```

Flash 实际占用约 221464/262144 字节，剩余约 40 KB。新增完整中文字库或大图片前必须重新评估空间；大资源计划存放到外部 W25Q128，但该模块尚未接入。

## UI 更新流程

LVGL Pro Editor 项目当前以 9.5 格式导出，生成的最小 C 代码已在本工程 LVGL 9.6 上通过编译。

1. 在 Editor 中修改 XML；
2. 按 `Ctrl+E` 仅导出 C 代码；
3. 将生成的 C/H 文件同步到 `User/ui/`；
4. 不直接修改名称包含 `_gen` 的文件；
5. Clean 并重新构建固件。

更多信息见 [架构](docs/architecture.md)、[接口](docs/protocol.md)、[任务](docs/tasks.md) 和 [调试记录](docs/debugging.md)。

## 下一阶段

1. 将 TFT SPI1 像素发送升级为 DMA；
2. 接入 W25Q128 并验证识别、擦除、写入和读取；
3. 建立 LVGL 文件系统接口，把字体和图片资源放入外部 Flash。
