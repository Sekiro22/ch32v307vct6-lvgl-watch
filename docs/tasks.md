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
| `UI_Thread` | `system_init()` | `configMAX_PRIORITIES - 4`，即 11 | 2048 个栈元素，约 8 KB | 初始化 LVGL、创建显示/UI、处理 SET/RET 通知并切屏、周期处理 LVGL |
| `key_task` | `ui_thread()` | `configMAX_PRIORITIES - 3`，即 12 | 256 个栈元素 | 阻塞接收按键事件，打印按键名称；SET/RET 通过任务通知交给 UI 任务 |

FreeRTOS 还会创建 Idle 任务；由于 `configUSE_TIMERS=1`，也会创建 Timer Service 任务，其优先级为 14、栈深度为 256 个栈元素。

## 软件定时器

| 定时器 | 创建位置 | 周期 | 主要职责 |
| --- | --- | --- | --- |
| `KeyScan` | `KEY_Init()` | 20 ms，自动重载 | 扫描 PE1～PE6；连续两次采样一致后更新稳定按键状态，仅在稳定按下沿发送按键消息 |

按键使用内部上拉，按下为低电平；`KEY_GetState()` 返回按下状态位，bit 0～5 分别对应 PE1～PE6。回调运行于 Timer Service 任务，队列发送不等待；队列满时本次按下事件会丢失。

`key_queue` 在 UI 任务中创建，容量为 3 个 `uint8_t` 消息；扫描定时器每 20 ms 采样一次，连续两次结果相同才确认状态变化。`key_task` 收到 PE2/SET 或 PE1/RET 后，用 `eSetBits` 任务通知 UI 任务；UI 任务在表盘按 SET 时创建并加载菜单，在菜单按 RET 时返回缓存的表盘屏幕。其余按键当前仍只打印，不调用 LVGL。

表针使用 LVGL 定时器（非 FreeRTOS 软件定时器）每 1000 ms 在 UI 任务中读取 `RTC_GetCounter()` 并更新三根 Line；它不需要 RTC 秒中断。

## UI 任务循环

```text
初始化 LVGL 和屏幕
  -> 检查 SET/RET 任务通知并切换屏幕
  -> lv_timer_handler()
  -> vTaskDelay(pdMS_TO_TICKS(2))
  -> 重复
```

LVGL Tick 由 `xTaskGetTickCount() * portTICK_PERIOD_MS` 提供，不需要在 SysTick 中额外调用 `lv_tick_inc()`。

## 共享资源约束

- 当前只有 UI 任务调用 LVGL，因此没有 LVGL Mutex。
- RTC 在调度器启动前初始化；当前时钟源为 LSI，后备寄存器标记控制首次设时，重启不应再次重置后备域。
- `TFT_init()` 在调度器启动前完成；运行期由 UI 任务启动 SPI DMA，DMA1 Channel 3 中断结束传输并通知 LVGL。
- LVGL 默认不是线程安全的。未来其他任务不得直接调用 LVGL API，应通过队列/事件把数据交给 UI 任务；如果确需跨任务调用，必须统一加锁。
- DMA 中断在传输计数完成后继续等待 SPI1 `BSY` 清零，再拉高 CS 并调用 `lv_display_flush_ready()`。
- 当前 `configCHECK_FOR_STACK_OVERFLOW=0`。UI 栈已临时增至约 8 KB，后续清理调试代码后应测量 high-water mark，再决定是否缩减。

## 后续任务建议

新增传感器、存储或通信功能时，优先采用“业务任务产生数据，UI 任务负责显示”的结构。不要在 UI 任务中执行长时间 Flash 擦写、网络等待或传感器阻塞采样。
