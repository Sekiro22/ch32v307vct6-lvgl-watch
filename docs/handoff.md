# 当前任务交接

## 当前任务

在 CH32V307VCT6 + FreeRTOS + LVGL 9.6 + GC9A01 工程中接入 SPI DMA，并将 LVGL Pro 生成的红黑圆形表盘稳定运行在实机上。

用户的固定工作流是：在上一级目录的 LVGL Pro 工程中设计界面、生成代码，再复制到固件工程并上机。未经用户明确确认，不得修改项目代码；`User/ui/**/*_gen.*` 只能通过 LVGL Pro 重新生成，不应手工编辑。

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
- 当前时、分、秒指针旋转值均为 0，画面已验证正常。

## 当前状态

- 分支：`main`；
- 工作区存在大量未提交修改，均应视为用户当前工作，不得覆盖、删除或回滚；
- 当前构建通过，产物时间为 2026-09-25 00:15:45；
- 最近构建大小：`text=232748`、`data=336`、`bss=53112`；
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
16 KB LVGL 堆不足
  -> 软件遮罩/变换绘制期间内存申请失败或状态损坏
  -> lv_draw_sw_mask_apply()/memcpy() 异常
  -> HardFault
  -> 旧画面闪烁或软件复位循环
```

## 未完成

- 当前指针角度均为 0，尚未实现真实时间驱动和动态角度更新；
- 尚未记录稳定界面下的 `lv_mem_monitor()` 峰值与最大连续空闲块；
- 尚未测量 UI 任务栈 high-water mark，8 KB 栈可能偏大；
- W25Q128、LVGL 文件系统、图片资源、输入设备和手表业务数据仍未接入；
- 已在本轮完成 Git commit 与 push。

## 下一步

1. 清理本轮临时诊断输出，再重新构建和上机回归；
2. 在稳定界面下测量 LVGL 堆峰值和 UI 栈 high-water mark；
3. 在 LVGL Pro 中恢复或重新设计指针。优先考虑不依赖旋转图层的 Line/表针方案，再生成代码上机；
4. 将步数、海拔、电量、心率和时间更新放在非生成用户代码中，避免下一次导出覆盖；
5. 检查完整 Git diff，确认无临时文件或无关修改后提交（本轮已由用户授权完成 commit 与 push）。

## 注意事项

- 当前仍保留临时诊断代码：启动时打印 RCC 复位原因、HardFault 打印寄存器并停机、UI 循环每次打印两行；最后一项会严重影响刷新性能，应优先清理，但必须先获得用户确认；
- `configCHECK_FOR_STACK_OVERFLOW` 仍为 0；
- LVGL Pro 项目使用 9.5 格式，固件为 LVGL 9.6，当前只有弃用警告，后续新增控件仍需逐次验证；
- 32 KB LVGL 堆使静态 RAM 占用升至 53112 字节，继续增加内存池前必须检查 map；
- 不要把业务逻辑写进 `_gen.*`；
- 不要在其他任务直接调用 LVGL，业务任务应通过队列/事件把数据交给 UI 任务。
