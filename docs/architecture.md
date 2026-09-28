# 项目架构

## 系统目标

工程在 CH32V307VCT6 上运行 FreeRTOS 和 LVGL，通过 SPI1 驱动 240×240 GC9A01 圆形 TFT。当前目标是建立稳定、可继续扩展的 UI 基线。

## 分层与职责

| 层 | 主要目录/文件 | 职责 |
| --- | --- | --- |
| 启动与芯片支持 | `Startup/`、`SRC/Core/`、`SRC/Peripheral/` | RISC-V 启动、中断和 WCH 外设库 |
| 系统配置 | `User/system_ch32v30x.c`、`Ld/Link.ld` | 144 MHz HSE 系统时钟、256 KB Flash/64 KB RAM 内存布局 |
| RTOS | `FreeRTOS/`、`User/FreeRTOSConfig.h` | 抢占式任务调度、12 KB heap_4 动态堆 |
| 显示驱动 | `Dev/TFT.c`、`Dev/TFT.h` | SPI1、GC9A01 初始化、窗口设置和 RGB565 像素发送 |
| 图形库 | `Middlewares/LVGL/`、`Middlewares/lv_conf.h` | LVGL 9.6 核心、软件渲染和控件 |
| LVGL 端口 | `User/app/ui_thread.c` | Tick、显示对象、局部缓冲、flush 回调和 LVGL 主循环 |
| UI | `User/ui/` | LVGL Pro 生成的项目初始化及屏幕创建代码 |
| 按键 | `Dev/key.c`、`User/app/app_key_event.c` | PE1～PE6 扫描、消抖、按下事件队列；SET/RET 通知 UI 任务切屏 |
| 时钟 | `User/app/watch_clock.c` | LSI 驱动 RTC 计数；在 UI 任务内用 LVGL Line 绘制并刷新表针 |

## 启动调用链

```text
Reset
  -> SystemInit / C runtime
  -> main()
     -> NVIC_PriorityGroupConfig()
     -> SystemCoreClockUpdate()
     -> Delay_Init()
     -> USART_Printf_Init(115200)
     -> system_init()
        -> TFT_init()
           -> SPI1/GPIO 初始化
           -> GC9A01 硬件复位与寄存器初始化
        -> RTC_Init()
           -> 启动 LSI；仅当后备标记不存在时配置 RTC 并设置初始时间
        -> xTaskCreate(ui_thread, ...)
     -> vTaskStartScheduler()
        -> ui_thread()
           -> KEY_Init()、创建按键队列
           -> lv_init()
           -> lv_tick_set_cb()
           -> lv_display_create(240, 240)
           -> 注册 RGB565 局部缓冲和 flush 回调
           -> ch32v307_gc9a01_ui_init("")
           -> 创建并加载表盘屏幕
           -> watch_clock_init()：创建 Line 表针及 1 秒 LVGL 定时器
           -> xTaskCreate(key_event_task, ...)
           -> 接收 SET/RET 任务通知并切换表盘/菜单
           -> 周期调用 lv_timer_handler()
```

## 显示数据流

LVGL 使用 240×10×2 字节（4800 字节）的单局部缓冲。发生刷新时：

```text
LVGL 软件渲染
  -> lvgl_flush(area, pixel_map)
  -> TFT_setWindow(x1, y1, x2, y2)
  -> GC9A01 2A/2B/2C 命令
  -> lv_draw_rgb565_swap()
  -> TFT_writePixelsDMA()
  -> DMA1 Channel 3 向 SPI1 DATAR 发送 RGB565 字节流
  -> DMA1_Channel3_IRQHandler()
     -> 等待 SPI1 BSY 清零
     -> CS 拉高，结束本次传输
     -> ui_flush_complete_from_isr()
     -> lv_display_flush_ready()
```

SPI1 当前使用二分频。DMA 完成只表示数据已搬入 SPI，仍需等待 SPI1 `BSY` 清零后才能拉高 CS。`lv_display_flush_ready()` 必须在本次 DMA 传输完成后调用，不能在 flush 回调中提前调用。

## 内存与资源

- 链接布局：256 KB Flash、64 KB RAM。
- LVGL 内部堆：32 KB。
- FreeRTOS heap_4：12 KB。
- UI 任务栈：2048 个 `StackType_t`，RV32 下约 8 KB，来自 FreeRTOS 堆。
- LVGL 绘图缓冲：4800 字节静态数组。
- 2026-09-28 构建：`text=245392`、`data=384`、`bss=53148`；Flash 余量约 16 KB。

内部 Flash 和 RAM 空间均已较紧张。新增资源时优先使用字体子集；大字体和图片后续应存放到 W25Q128，并通过 LVGL 文件系统读取。增加控件或旋转/缩放对象后，需要重新检查 LVGL 堆峰值和最大连续空闲块。

## 生成代码边界

- `User/ui/*_gen.c`、`User/ui/*_gen.h` 和 `User/ui/screens/*_gen.*` 由 LVGL Pro 生成，重新导出时会覆盖。
- 菜单 XML 与字体源位于同级 LVGL Pro 工程 `../ch32v307_gc9a01_ui/ch32v307_gc9a01_ui`；固件仓库目前保存导出的 C/H 和字体数据 C 文件。
- `User/ui/ch32v307_gc9a01_ui.c` 是用户扩展入口，生成器只在文件不存在时创建骨架。
- 驱动、RTOS 和 LVGL 初始化不属于生成 UI，应继续保留在 `Dev/` 与 `User/app/`。

## 当前实现边界

- SET（PE2）从表盘进入一级菜单，RET（PE1）从菜单返回表盘；UP/DOWN 轮询选择及菜单项功能尚未实现，触摸输入未实现；
- 表针每秒读取 RTC 计数，当前使用标称 40 kHz 的内部 LSI 和预分频 39999。首次初始化时间仍为代码中的固定值，尚无用户校时入口；LSI 精度及断电保持未验证；
- 双缓冲；
- W25Q128 驱动及 LVGL 文件系统；
- 更多中文字体资源；当前只有一级菜单用的 14/16/20 px 字形子集；
- 低功耗、传感器和其他手表业务功能。
