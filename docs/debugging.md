# 调试记录

## 构建与配置

### LVGL 公共头文件找不到

症状：

```text
fatal error: ../include/lvgl/lvgl.h: No such file or directory
```

根因是只复制了部分 LVGL 目录，`lvgl_public.h` 依赖完整的 `include/lvgl` 布局。当前工程已经保留 LVGL 9.6 的 `src/`、`include/`、`lvgl.h` 和 `lvgl_private.h`，并在 MounRiver 配置中加入相应 Include Path。

### Flash 溢出

曾出现 `.text` 无法放入 `FLASH`。当前 `Ld/Link.ld` 已按 CH32V307VCT6 配置为 256 KB Flash、64 KB RAM，并关闭不需要的 LVGL 功能/颜色格式。最近构建 Flash 占用约 221464 字节，仅余约 40 KB。

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

根因是 16 KB LVGL 内存池不足。新表盘包含圆角、刻度、圆弧、字体和旋转对象；软件遮罩及变换图层会继续申请 LVGL 内存。内存断言和日志关闭时，申请失败可能表现为描述符损坏、对象不显示或 HardFault，而不是清晰的 OOM 信息。

将 `LV_MEM_SIZE` 从 16 KB 增至 32 KB 后，表盘已在实机稳定刷新。排查同类问题时：

1. 先确认 HardFault 的 `mepc`、`mcause`、`mtval`；
2. 使用与烧录固件同一次构建的 ELF 和 `addr2line` 反查 `mepc`；
3. 使用 `lv_mem_monitor()` 检查 `free_size`、`free_biggest_size` 和 `max_used`；
4. 旋转、缩放对象需要临时图层，普通对象能显示不代表变换图层一定能分配成功；
5. 调整内存池后重新检查链接 map，确保 64 KB RAM 布局仍有余量。

当前生成代码中的时、分、秒指针旋转值均为 0，因此不再触发旋转图层；后续恢复动态角度时需要再次验证 LVGL 堆峰值。

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
