# 当前任务交接

## 当前任务

建立 CH32V307VCT6 + FreeRTOS + LVGL + GC9A01 的可继续开发基线，并完成 Git 备份与项目文档。

## 已完成

- GC9A01 SPI1 驱动已工作，方向值为 `MADCTL=0x48`；
- LVGL 9.6 已完成显示端口和 FreeRTOS Tick 接入；
- LVGL Pro 最小生成 UI 已接入；
- 当前工程完整构建通过；
- 功能基线已经提交并推送到 GitHub；
- README、架构、接口、任务和调试文档已建立。

## 当前状态

- 分支：`main`；
- 远程：`origin` 指向 `Sekiro22/ch32v307vct6-lvgl-watch`；
- 最近构建：`text=221128`、`data=336`、`bss=36716`；
- 屏幕能正常显示生成 UI；
- 构建产物位于被忽略的 `obj/`。

## 关键上下文

- 系统入口：`User/main.c`；
- UI 任务与 LVGL 端口：`User/app/ui_thread.c`；
- GC9A01 驱动：`Dev/TFT.c`；
- LVGL 配置：`Middlewares/lv_conf.h`；
- 生成 UI：`User/ui/`；
- 链接脚本：`Ld/Link.ld`。

## 未完成

- W25Q128 驱动与 LVGL 文件系统；
- 中文字体和图片资源管理；
- 输入设备；
- SPI DMA；
- 手表业务页面与数据更新机制。

## 下一步

1. 将 TFT SPI1 像素发送升级为 DMA，并在 DMA 完成后调用 `lv_display_flush_ready()`；
2. 接入 W25Q128，依次完成 JEDEC ID、擦除、写入和读取测试；
3. 为 W25Q128 注册 LVGL 文件系统驱动；
4. 将字体和图片资源转换后写入外部 Flash，并验证 LVGL 按文件加载；
5. 每个阶段重新检查 Flash、RAM、UI 任务栈和刷新稳定性。

## 注意事项

- 生成文件 `_gen.*` 不应手工修改；
- 只有 UI 任务可以直接调用 LVGL；
- 当前 Flash 余量约 40 KB；
- 屏幕固定亮线为已知硬件疑点，当前不处理。
