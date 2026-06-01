# HAL Platform 开发约束

## 概述

本项目为 **跨平台硬件抽象层(HAL)库**，核心约束位于 `subprojects/HAL/Platform/` 目录。所有平台实现必须遵循统一的设计模式与约束规范。

---

## 核心约束

### 1. 文件结构约束

```
Platform/
├── gd32f4.hpp    # GD32F4xx 平台 (主推)
├── stm32.hpp    # STM32 平台
└── mspm0.hpp    # TI MSPM0 平台
```

**新增平台必须**：创建新文件 `Platform/<platform>.hpp`，并在 `hal.hpp` 中添加对应 `#include`。

---

### 2. 命名空间约束

每个平台必须使用独立的命名空间：

```cpp
namespace HAL
{
    namespace <platform>  // 例: gd32f4, stm32, mspm0
    {
        // 实现
    }
}
```

---

### 3. 组件约束 (必须实现的模板)

所有平台必须实现以下组件模板，接口签名必须保持一致。

#### 3.1 GPIO 通用输入/输出

**必须方法**：

| 方法 | 返回类型 | 参数 | 说明 |
|------|----------|------|------|
| `init()` | `void` | — | 初始化 GPIO，含时钟使能 |
| `set()` | `void` | — | 置位引脚 |
| `clear()` | `void` | — | 清零引脚 |
| `toggle()` | `void` | — | 翻转引脚 |
| `read()` | `bool` | — | 读取引脚状态 |

**特化模式**（GD32F4xx）：
- `OutputConfig<OTYPE, SPEED, VAL>` — 输出模式
- `AFConfig<AF_NUM>` — 复用功能模式
- `void` — 输入模式

**其他平台**：可使用简化版本，无需特化区分。

---

#### 3.2 TIM 定时器

**必须方法**：

| 方法 | 返回类型 | 参数 | 说明 |
|------|----------|------|------|
| `init()` | `void` | — | 初始化定时器 |
| `start()` | `void` | — | 启动定时器 |
| `start_it()` | `void` | — | 启动定时器中断 |
| `set_prescaler()` | `void` | `uint32_t prescaler` | 设置预分频 |
| `set_autoreload()` | `void` | `uint32_t autoreload` | 设置自动重装载值 |
| `set_counter()` | `void` | `uint32_t counter` | 设置计数器值 |
| `get_autoreload()` | `uint32_t` | — | 获取自动重装载值 |
| `get_counter()` | `uint32_t` | — | 获取当前计数器值 |

---

#### 3.3 PWM 脉冲宽度调制

**必须方法**：

| 方法 | 返回类型 | 参数 | 说明 |
|------|----------|------|------|
| `init()` | `void` | — | 初始化 PWM，设置频率和占空比初值 |
| `start()` | `void` | — | 启动 PWM 输出 |
| `set_compare()` | `void` | `uint32_t compare` | 设置捕获比较值（占空比） |
| `set_frequency()` | `void` | `uint32_t frequency` | 设置 PWM 频率 |

**频率约束**：
```cpp
static_assert(!(frequency_ == 0 || frequency_ > 1000000), "PWM frequency must be in range 1-1000000 Hz");
```

---

#### 3.4 I2C_device_7bits I2C 主设备

**必须方法**：

| 方法 | 参数 | 说明 |
|------|------|------|
| `init()` | — | 初始化 I2C，引脚配置 |
| `transmit()` | `uint8_t* pData, uint16_t Size, uint32_t Timeout` | 发送数据 |
| `receive()` | `uint8_t* pData, uint16_t Size, uint32_t Timeout` | 接收数据 |
| `mem_write()` | `uint16_t MemAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout` | 写入寄存器 |
| `mem_read()` | `uint16_t MemAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout` | 读取寄存器 |

**约束**：
- 设备地址为 **7位地址**（不含读/写位）
- SDA/SCL 引脚必须配置为 **AF4** 复用功能
- 使用 `requires` 约束在编译时检查引脚配置

---

#### 3.5 可选组件

根据平台能力添加，以下为接口规范：

**UART 串口通信**：

| 方法 | 返回类型 | 参数 | 说明 |
|------|----------|------|------|
| `init()` | `void` | — | 初始化 UART，波特率等配置 |
| `transmit()` | `void` | `const uint8_t* pData, uint16_t Size, uint32_t Timeout` | 发送数据 |
| `receive()` | `void` | `uint8_t* pData, uint16_t Size, uint32_t Timeout` | 接收数据 |


**SPI 主/从设备**：

| 方法 | 返回类型 | 参数 | 说明 |
|------|----------|------|------|
| `init()` | `void` | — | 初始化 SPI，模式、速率等配置 |
| `transmit()` | `void` | `uint8_t* pData, uint16_t Size, uint32_t Timeout` | 发送数据 |
| `receive()` | `void` | `uint8_t* pData, uint16_t Size, uint32_t Timeout` | 接收数据 |
| `transfer()` | `void` | `uint8_t* pData, uint16_t Size, uint32_t Timeout` | 全双工传输 |

**ADC 模数转换**：

| 方法 | 返回类型 | 参数 | 说明 |
|------|----------|------|------|
| `init()` | `void` | — | 初始化 ADC，采样率等配置 |
| `start()` | `void` | — | 启动 ADC 转换 |
| `start_it()` | `void` | — | 启动 ADC 中断转换 |
| `get_value()` | `uint32_t` | — | 获取转换结果 |
| `get_channel()` | `uint32_t` | — | 获取当前通道 |

**CRC 校验**：

| 方法 | 返回类型 | 参数 | 说明 |
|------|----------|------|------|
| `init()` | `void` | — | 初始化 CRC 多项式等配置 |
| `calculate()` | `uint32_t` | `uint8_t* pData, uint16_t Size` | 计算 CRC 值 |
| `reset()` | `void` | — | 重置 CRC 状态 |

---

### 4. GPIO 模板变体示例

GD32F4xx 平台展示了三种 GPIO 特化模式，新增平台应参考：

```cpp
// 变体1: 输出模式 (OutputConfig 特化)
template <...>
struct GPIO<GPIOx, Pin, GPIO_MODE_OUTPUT, PULL, OutputConfig<...>> { ... };

// 变体2: 复用功能模式 (AFConfig 特化)
template <...>
struct GPIO<GPIOx, Pin, GPIO_MODE_AF, PULL, AFConfig<...>> { ... };

// 变体3: 输入模式
template <...>
struct GPIO<GPIOx, Pin, GPIO_MODE_INPUT, PULL, void> { ... };
```

**其他平台可简化**：STM32/MSPM0 使用简单 GPIO 模板，不做特化区分。

---

### 5. 编译时检查约束

使用 `static_assert` 进行编译时类型检查：

```cpp
// GD32F4xx 示例
static_assert((false), "fallback");  // 防止实例化未定义的特化

// PWM 频率约束
static_assert(!(frequency_ == 0 || frequency_ > 1000000), "PWM frequency must be in range 1-1000000 Hz");
```

---

### 6. HAL 设计原则

**HAL = Hardware Abstraction Layer（硬件抽象层）**

- ✅ **必须包含**: 外设寄存器操作、GPIO 配置、时钟使能、中断配置
- ❌ **禁止包含**: 业务逻辑、算法实现、应用层代码

**正确示例**：
```cpp
// HAL: 只做硬件初始化
struct GPIO { static void init() { /* 寄存器配置 */ } };
```

**错误示例**：
```cpp
// ❌ HAL 中包含业务逻辑
struct RTC {
    static void set_time(uint8_t year, uint8_t month, uint8_t day) {
        // 计算星期几...  ← 业务逻辑，不属于 HAL
    }
};
```

**业务逻辑应放在 example/ 或应用层**，不在 HAL 中。
