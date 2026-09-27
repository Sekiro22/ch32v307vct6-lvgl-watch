# 当前任务交接

## 当前任务

CH32V307VCT6 + FreeRTOS + LVGL 9.6 + GC9A01 的显示、按键和 RTC 表针基础已接入；下一阶段准备做一级菜单及按键导航。

用户的固定工作流是：在上一级目录的 LVGL Pro 工程中设计界面、生成代码，再复制到固件工程并上机。`User/ui/**/*_gen.*` 只能通过 LVGL Pro 重新生成，不应手工编辑。

## 已完成

- SPI1 分频由 8 调整为 2，实机验证显示正常；
- SPI1 TX 使用 DMA1 Channel 3，局部刷新不再阻塞发送全部像素；
- flush 前执行 RGB565 字节交换；
- DMA 完成中断等待 SPI `BSY` 清零后拉高 CS，再调用 `lv_display_flush_ready()`；
- LVGL Pro 表盘已包含外圈、分钟刻度、电量弧、电量文本、步数、海拔、心率、时分秒指针及中心圆点；
- 新增 Montserrat 10 px、12 px 字体子集，字体源文件位于 `User/ui/fonts/`；
- 解决生成 UI 加入字体后的链接问题；
- 通过 HardFault 的 `mepc/mcause/mtval` 和 ELF 反查定位首次渲染崩溃；
- 将 `LV_MEM_SIZE` 从 16 KB 调整到 32 KB 后，表盘已在实机稳定刷新；
- PE1～PE6 内部上拉输入、20 ms 周期扫描与按下事件队列已接入；`key_task` 当前仅打印按键名称；
- 生成矩形指针固定角度为 0；`User/app/watch_clock.c` 创建三根 LVGL Line 指针，每秒读取 RTC 计数更新表盘，未改生成文件；
- RTC 已切到内部 LSI，预分频 39999。后备标记 `BKP_DR1=0xA1A1` 控制首次设时，启动时不再执行 `BKP_DeInit()`；用户已烧录测试 RTC 方案，但长期精度和断电保持尚无验证记录。

## 当前状态

- 分支：`main`；
- 2026-09-27 使用 MounRiver 工具链构建通过；
- 构建有 `watch_clock_update()` 中 `elapsed` 未使用的警告；按用户要求保留原代码，未做清理；
- 最近构建大小：`text=236896`、`data=384`、`bss=53136`；
- `.claude/settings.local.json` 为本地未跟踪配置，不属于固件版本；
- 链接布局仍为 256 KB Flash、64 KB RAM；
- LVGL 内部堆：32 KB；
- FreeRTOS heap_4：12 KB；
- UI 任务栈：2048 个 32 位栈元素，约 8 KB；
- LVGL 单缓冲：240×10×RGB565，共 4800 字节；
- 实机状态：画面可以正常刷新，不再反复复位。

## 关键上下文

- 入口与 DMA 中断：`User/main.c`；
- UI 任务、flush 和 DMA 完成回调：`User/app/ui_thread.c`；
- SPI/DMA/GC9A01 驱动：`Dev/TFT.c`；
- LVGL 配置：`Middlewares/lv_conf.h`；
- 当前生成表盘：`User/ui/screens/screen_main_gen.c`；
- 用户时钟逻辑与 Line 指针：`User/app/watch_clock.c`，由 `ui_thread.c` 在加载主屏后调用 `watch_clock_init()`；
- RTC 初始化：`main.c` 的 `system_init()` 调用 `RTC_Init()`；按键：`Dev/key.c`、`User/app/app_key_event.c`；
- 字体：`User/ui/fonts/font_data_10_data.c`、`font_data_12_data.c`；
- HardFault 诊断：`User/ch32v30x_it.c`；
- 复位原因诊断：`SRC/Debug/debug.c`。

显示完成链路：

```text
lv_timer_handler()
  -> LVGL 软件渲染到 240×10 缓冲
  -> lvgl_flush()
  -> TFT_setWindow()
  -> lv_draw_rgb565_swap()
  -> TFT_writePixelsDMA()
  -> DMA1_Channel3_IRQHandler()
  -> 等待 SPI1 BSY 清零
  -> CS 拉高
  -> ui_flush_complete_from_isr()
  -> lv_display_flush_ready()
```

首次刷新崩溃的诊断链路：

```text
16 KB LVGL 堆下首次绘制失败（疑似内存压力）
  -> lv_draw_sw_mask_apply()/memcpy() 发生访问异常
  -> HardFault
  -> 旧画面闪烁或软件复位循环
```

将 LVGL 堆增至 32 KB、三个生成指针旋转角度设为 0 后，用户确认画面正常；随后确认非零旋转时分、秒针不可见，固定为 0 时恢复。当前以 Line 替代生成矩形指针，并通过 RTC 计数刷新角度。

## 未完成

- 一级菜单及按键导航尚未实现；按键任务当前只打印事件；
- RTC 初始时间仍硬编码，缺少用户校时入口；LSI 走时偏差需实测校准，断电后的时间保持未验证；
- Line 指针的长期运行、内存峰值和刷新残影仍需回归；
- 尚未记录稳定界面下的 `lv_mem_monitor()` 峰值与最大连续空闲块；
- 尚未测量 UI 任务栈 high-water mark，8 KB 栈可能偏大；
- W25Q128、LVGL 文件系统、图片资源、传感器和其他手表业务数据仍未接入。

## 下一步

1. 在 LVGL Pro 设计一级菜单并导出生成代码；先确定菜单项、页面关系和六个按键的操作映射，再让 UI 任务消费按键事件；
2. 保持菜单逻辑在非生成用户代码中，避免下一次 LVGL Pro 导出覆盖；
3. 回归 RTC 复位保持、表针长期刷新与 LSI 误差；加入校时入口前不要把硬编码初始时间当作实时时钟；
4. 测量 LVGL 堆峰值和 UI 栈 high-water mark；后续再接 W25Q128、文件系统及业务数据。

## 注意事项

- RTC 失败时 `system_init()` 会直接返回、不创建 UI 任务；设计菜单前应先确认当前 RTC 可以稳定初始化；
- 按键队列容量为 3，满队列会丢弃本次事件；未来菜单交互需验证长按/连按需求；
- `configCHECK_FOR_STACK_OVERFLOW` 仍为 0；
- LVGL Pro 项目使用 9.5 格式，固件为 LVGL 9.6，当前只有弃用警告，后续新增控件仍需逐次验证；
- 32 KB LVGL 堆使静态 RAM 占用升至 53136 字节，继续增加内存池前必须检查 map；
- 不要把业务逻辑写进 `_gen.*`；
- 不要在其他任务直接调用 LVGL，业务任务应通过队列/事件把数据交给 UI 任务。
