# HAL 开发约束

HAL 为 Hardware 提供统一的静态接口。新增代码和修改旧代码都必须遵循本文件，不再增加同一功能的第二套接口。

## 通用规则

- 平台代码放在 `Platform/<platform>.hpp`，命名空间为 `HAL::<platform>`。
- 使用 `<stdint.h>` 和 `uint*_t`；只读缓冲区使用 `const uint8_t*`。
- 外设配置放在模板参数或配置类型中，`init()` 不接收运行时配置。
- 非法模板参数应通过 `static_assert`、`requires` 或 concept 在编译期报错。

## 统一接口

### GPIO

| 模式 | 接口 |
| --- | --- |
| 输入 | `init()`、`read()` |
| 输出、复用 | `init()`、`set()`、`clear()`、`toggle()`、`read()` |
| 模拟 | `init()` |

`read()` 返回 `bool`。

### TIM

```cpp
static void init();
static void start();
static void start_it();
static void set_prescaler(uint32_t value);
static void set_autoreload(uint32_t value);
static void set_counter(uint32_t value);
static uint32_t get_clock_frequency();
static uint32_t get_autoreload();
static uint32_t get_counter();
```

### PWM

```cpp
static void init();
static void start();
static void stop();
static void set_compare(uint32_t value);
static uint32_t get_autoreload();
```

`init()` 只完成配置，`start()` 才启动输出。运行时改频不是通用接口。

### I2C

地址统一使用未移位的 7-bit 地址。

```cpp
// I2C_bus
static void init();
static void transmit(uint8_t address, const uint8_t* data,
                     uint16_t size, uint32_t timeout);
static void receive(uint8_t address, uint8_t* data,
                    uint16_t size, uint32_t timeout);

// I2C_device<bus_t, address>
static void init();
static void transmit(const uint8_t* data, uint16_t size,
                     uint32_t timeout);
static void receive(uint8_t* data, uint16_t size,
                    uint32_t timeout);
static void mem_write(uint16_t reg, const uint8_t* data,
                      uint16_t size, uint32_t timeout);
static void mem_read(uint16_t reg, uint8_t* data,
                     uint16_t size, uint32_t timeout);
```

寄存器地址统一为 8-bit，`reg` 仅使用低 8 位。接口中不出现厂商专用的地址宽度参数。

### UART

```cpp
static void init();
static void transmit(const uint8_t* data, uint16_t size,
                     uint32_t timeout);
static void receive(uint8_t* data, uint16_t size,
                    uint32_t timeout);
```

### SPI

SPI 总线不管理 CS：

```cpp
static void init();
static void transmit(const uint8_t* data, uint16_t size,
                     uint32_t timeout);
static void receive(uint8_t* data, uint16_t size,
                    uint32_t timeout);
static void transfer(uint8_t* data, uint16_t size,
                     uint32_t timeout);
```

`SPI_device<bus_t, gpio_cs>` 另外提供：

```cpp
static void select();
static void deselect();
static void transmit_without_ctl_select(const uint8_t* data,
                                         uint16_t size, uint32_t timeout);
static void receive_without_ctl_select(uint8_t* data,
                                       uint16_t size, uint32_t timeout);
static void transfer_without_ctl_select(uint8_t* data,
                                        uint16_t size, uint32_t timeout);
```

设备层普通收发自动控制 CS；`*_without_ctl_select()` 不改变 CS。

### ADC

```cpp
static void init();
static void start();
static void start_it();
static uint32_t get_value();
static uint32_t get_channel();
```

### CRC

```cpp
static void init();
static uint32_t calculate(const uint8_t* data, uint16_t size);
static void reset();
```
