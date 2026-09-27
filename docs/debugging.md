# 调试记录

## 构建与配置

### LVGL 公共头文件找不到

症状：

```text
fatal error: ../include/lvgl/lvgl.h: No such file or directory
```

根因是只复制了部分 LVGL 目录，`lvgl_public.h` 依赖完整的 `include/lvgl` 布局。当前工程已经保留 LVGL 9.6 的 `src/`、`include/`、`lvgl.h` 和 `lvgl_private.h`，并在 MounRiver 配置中加入相应 Include Path。

### Flash 溢出

曾出现 `.text` 无法放入 `FLASH`。当前 `Ld/Link.ld` 已按 CH32V307VCT6 配置为 256 KB Flash、64 KB RAM，并关闭不需要的 LVGL 功能/颜色格式。2026-09-27 构建 `text + data = 237280` 字节，256 KB Flash 理论余量约 24 KB；新增资源前仍应以 map 和实际烧录结果复核。

处理新增功能时应先检查 size 输出。不要直接加入完整中文字库、Demo 或大图片数组。

### 命令行找不到 size 工具

症状：

```text
riscv-none-embed-size: not found
```

这是终端 PATH 问题，不是源码错误。使用 MounRiver Studio 构建，或把其 `risc-v embedded gcc/bin` 目录加入当前终端 PATH。

## 显示问题

### 文字镜像

GC9A01 的 `MADCTL (0x36)` 当前使用 `0x48`。此前方向值导致文字镜像。若更改屏幕安装方向，需要系统性验证旋转、RGB/BGR 顺序和 LVGL 坐标，不能只根据纯色测试判断。

### 花屏后黑屏或标签不显示

显示驱动基本正常但 LVGL 对象未正常显示时，应依次检查：

1. `lv_timer_handler()` 是否持续运行；
2. Tick 是否以毫秒返回；当前 500 Hz Tick 需要乘 `portTICK_PERIOD_MS`；
3. RGB565 是否按高字节、低字节发送；
4. flush 结束后是否调用 `lv_display_flush_ready()`；
5. LVGL 内部堆是否足够。当前 `LV_MEM_SIZE` 为 32 KB。

### 复杂表盘首次刷新时 HardFault 或反复复位

症状包括旧画面闪烁、启动日志反复出现，或首次进入 `lv_timer_handler()` 后触发 HardFault。曾捕获到两类异常：

```text
mcause=4，mepc 位于 lv_draw_sw_mask_apply()
mcause=5，mepc 位于 memcpy()，mtval=0x20010000
```

当前证据指向 LVGL 绘制内存不足：新表盘包含圆角、刻度、圆弧、字体和旋转对象；软件遮罩及变换图层会继续申请 LVGL 内存。内存断言和日志关闭时，申请失败可能表现为对象不显示或 HardFault。异常时未记录内存监控数据，因此尚不能证明每次异常都由同一个分配失败直接触发。

将 `LV_MEM_SIZE` 从 16 KB 增至 32 KB，且将三个指针旋转角度设为 0 后，用户确认表盘正常。两项变化的独立作用尚未做单变量回归。排查同类问题时：

1. 先确认 HardFault 的 `mepc`、`mcause`、`mtval`；
2. 使用与烧录固件同一次构建的 ELF 和 `addr2line` 反查 `mepc`；
3. 使用 `lv_mem_monitor()` 检查 `free_size`、`free_biggest_size` 和 `max_used`；
4. 旋转、缩放对象需要临时图层，普通对象能显示不代表变换图层一定能分配成功；
5. 调整内存池后重新检查链接 map，确保 64 KB RAM 布局仍有余量。

当前生成代码中的时、分、秒指针旋转值均为 0。用户确认非零旋转时分、秒针不可见、固定为 0 后恢复；现由 `User/app/watch_clock.c` 隐藏生成矩形指针并创建三根 Line，避免旋转图层。Line 动态方案的长期内存峰值和残影情况仍需实机回归。

## RTC 走时与保留

- 当前使用内部 LSI。它的标称频率约 40 kHz；误用 LSE 的 `32767` 预分频会使 RTC 明显走快，现改为 `39999` 作粗调。LSI 个体及温度误差仍需实测校准，不能据此保证长期精度。
- `RTC_Init()` 在启动 UI 任务前运行：每次启动使能 LSI；仅后备寄存器 `BKP_DR1` 不等于 `0xA1A1` 时选择 RTC 时钟源、设分频和写入代码中的固定初始时间。每次启动调用 `BKP_DeInit()` 或 `RTC_Set()` 都会破坏重启后保留时间的目标。
- 先前 LSE 方案报 `RTC open error!` 时，失败点是等待 `LSERDY` 超时；该项目中未提供板级原理图，不能仅凭代码判断 32.768 kHz 晶振是否存在或起振。LSI 方案在复位/烧录且持续供电时的时间保留应实测；VDD 完全断电后继续走时未保证，需确认 VBAT 与 LSE 硬件方案。

### 屏幕固定亮线

屏幕从首次上电开始就存在固定亮线，且与填充颜色和 LVGL 内容无关。现阶段判断更像面板、排线或模组硬件缺陷，暂不作为软件问题处理。

## LVGL Pro 生成代码

- Editor 项目使用其支持的 LVGL 9.5 格式；本工程使用 LVGL 9.6。当前最小生成代码已经通过编译，但增加新组件后仍需逐次构建验证兼容性。
- 只导出代码使用 `Ctrl+E`，不要求 EMSDK。
- 锤子、`Ctrl+B` 或 Clean/Rebuild 会重编译预览，需要 EMSDK 4.0.6。
- `_gen.c/.h` 会被重新导出覆盖，业务逻辑不要写入这些文件。
- 生成代码使用对象名称与翻译模块，因此 `LV_USE_OBJ_NAME` 和 `LV_USE_TRANSLATION` 当前均启用。

## 圆形屏幕布局

GC9A01 逻辑坐标仍为 240×240，编辑器显示方形画布属于正常现象。主要矩形控件放在中央约 170×170 区域（大致 X/Y 为 35～205）可避免被圆形边缘裁掉。屏幕四角使用黑色背景即可，无需为物理不可见区域增加运行时圆角裁剪。
