# 当前任务交接

## 当前任务

CH32V307VCT6 + FreeRTOS + LVGL 9.6 + GC9A01 的显示、按键和 RTC 表针基础已接入；一级菜单静态界面及 SET/RET 切屏已接入，下一阶段实现 UP/DOWN 轮询选择。

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
- PE1～PE6 内部上拉输入、20 ms 周期扫描与按下事件队列已接入；`key_task` 打印按键名称，并将 SET/RET 通过任务通知交给 UI 任务；
- 生成矩形指针固定角度为 0；`User/app/watch_clock.c` 创建三根 LVGL Line 指针，每秒读取 RTC 计数更新表盘，未改生成文件；
- RTC 已切到内部 LSI，预分频 39999。后备标记 `BKP_DR1=0xA1A1` 控制首次设时，启动时不再执行 `BKP_DeInit()`；用户已烧录测试 RTC 方案，但长期精度和断电保持尚无验证记录。
- LVGL Pro 工程 `../ch32v307_gc9a01_ui/ch32v307_gc9a01_ui` 新增 `screens/screen_menu.xml`：一级菜单按“设置、秒表、计时器、心率、锻炼、闹钟”循环，固定显示 5 行，选中项居中；初始画面从上到下为“锻炼、闹钟、设置、秒表、计时器”。中文使用 14/16/20 px 的加粗字形子集 `fonts/NotoSansSC-SemiBold-menu.ttf`，没有“菜单”标题。
- LVGL Pro 已生成菜单 C/H 和 3 个字体数据 C 文件并同步到 `User/ui/`；表盘按 SET（PE2）加载菜单、菜单按 RET（PE1）返回表盘。UI 任务缓存两个屏幕指针，避免重复创建表盘或菜单。

## 当前状态

- 分支：`codex/menu-navigation`；
- 2026-09-28 使用 MounRiver 工具链增量构建通过；菜单尚未上机验证；
- 任务开始前已有 `.template`、`User/app/watch_clock.c` 的未提交修改和未跟踪 `.claude/`；本次版本不包含这些文件；
- 最近构建大小：`text=245392`、`data=384`、`bss=53148`，Flash 余量约 16 KB；
- `.claude/settings.local.json` 为本地未跟踪配置，不属于固件版本；
- 链接布局仍为 256 KB Flash、64 KB RAM；
- LVGL 内部堆：32 KB；
- FreeRTOS heap_4：12 KB；
- UI 任务栈：2048 个 32 位栈元素，约 8 KB；
- LVGL 单缓冲：240×10×RGB565，共 4800 字节；
- 实机状态：画面可以正常刷新，不再反复复位。
- 菜单 XML、字形和 240×240 圆屏静态预览已检查；生成代码已在固件编译通过，尚未上机测试。LVGL Pro 工程本身不是 Git 仓库，其 XML 与 TTF 源未纳入固件仓库。

## 关键上下文

- 入口与 DMA 中断：`User/main.c`；
- UI 任务、flush 和 DMA 完成回调：`User/app/ui_thread.c`；
- SPI/DMA/GC9A01 驱动：`Dev/TFT.c`；
- LVGL 配置：`Middlewares/lv_conf.h`；
- 当前生成表盘：`User/ui/screens/screen_main_gen.c`；
- 当前生成菜单：`User/ui/screens/screen_menu_gen.c`；菜单字体：`User/ui/fonts/font_menu_{14,16,20}_data.c`；
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

- 一级菜单 UP/DOWN 六项轮询选择及各菜单项功能尚未实现；目前只有 SET 进入、RET 返回；
- SET/RET 切屏和菜单内存占用仍需实机回归；
- RTC 初始时间仍硬编码，缺少用户校时入口；LSI 走时偏差需实测校准，断电后的时间保持未验证；
- Line 指针的长期运行、内存峰值和刷新残影仍需回归；
- 尚未记录稳定界面下的 `lv_mem_monitor()` 峰值与最大连续空闲块；
- 尚未测量 UI 任务栈 high-water mark，8 KB 栈可能偏大；
- W25Q128、LVGL 文件系统、图片资源、传感器和其他手表业务数据仍未接入。

## 下一步

1. 在 UI 任务中实现菜单索引按 6 项循环，固定显示选中项前后各 2 项；UP/DOWN 更新 5 个标签文本，并上机验证 SET/RET 切屏；
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
