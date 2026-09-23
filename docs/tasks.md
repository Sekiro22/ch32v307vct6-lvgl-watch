# FreeRTOS 任务与并发

## 调度配置

| 配置 | 当前值 |
| --- | --- |
| 调度方式 | 抢占式（`configUSE_PREEMPTION=1`） |
| Tick 频率 | 500 Hz，即 2 ms/tick |
| 最大优先级数量 | 15 |
| FreeRTOS 堆 | 12 KB，heap_4 |
| Mutex | 启用 |
| 软件定时器 | 启用 |
| 栈溢出检测 | 未启用 |

## 应用任务

| 任务 | 创建位置 | 优先级 | 栈深度 | 主要职责 |
| --- | --- | --- | --- | --- |
| `UI_Thread` | `system_init()` | `configMAX_PRIORITIES - 4`，即 11 | 1024 个栈元素，约 4 KB | 初始化 LVGL、创建显示/UI、周期处理 LVGL |

FreeRTOS 还会创建 Idle 任务；由于 `configUSE_TIMERS=1`，也会创建 Timer Service 任务，其优先级为 14、栈深度为 256 个栈元素。

## UI 任务循环

```text
初始化 LVGL 和屏幕
  -> lv_timer_handler()
  -> vTaskDelay(pdMS_TO_TICKS(5))
  -> 重复
```

LVGL Tick 由 `xTaskGetTickCount() * portTICK_PERIOD_MS` 提供，不需要在 SysTick 中额外调用 `lv_tick_inc()`。

## 共享资源约束

- 当前只有 UI 任务调用 LVGL，因此没有 LVGL Mutex。
- `TFT_init()` 在调度器启动前完成；运行期显示访问来自 UI 任务。
- LVGL 默认不是线程安全的。未来其他任务不得直接调用 LVGL API，应通过队列/事件把数据交给 UI 任务；如果确需跨任务调用，必须统一加锁。
- 当前 SPI1 为同步阻塞访问。增加 DMA 后，必须在 DMA 完成中断或完成通知中调用 `lv_display_flush_ready()`，不能像现在一样立即调用。

## 后续任务建议

新增传感器、存储或通信功能时，优先采用“业务任务产生数据，UI 任务负责显示”的结构。不要在 UI 任务中执行长时间 Flash 擦写、网络等待或传感器阻塞采样。
