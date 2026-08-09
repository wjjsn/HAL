#pragma once


#ifdef __cplusplus
#include <stdint.h>

#include "../hal.hpp"

#ifdef STM32F1
#include "stm32f1xx.h"
#include "stm32f1xx_ll_usart.h"
#endif // STM32F1
namespace HAL
{
	namespace stm32
	{
		template <uint32_t GPIOx, uint16_t GPIO_Pin, typename config>
		struct GPIO
		/*todo:
		 *代替HAL库实现更好性能
		 *进行编译时检查
		 */
		{
			static void init()
			{
				config::init();
			}
			static void set()
			{
				HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef *>(GPIOx), GPIO_Pin, GPIO_PIN_SET);
			}
			static void clear()
			{
				HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef *>(GPIOx), GPIO_Pin, GPIO_PIN_RESET);
			}
			static void toggle()
			{
				HAL_GPIO_TogglePin(reinterpret_cast<GPIO_TypeDef *>(GPIOx), GPIO_Pin);
			}
			static bool read()
			{
				return HAL_GPIO_ReadPin(reinterpret_cast<GPIO_TypeDef *>(GPIOx), GPIO_Pin) == GPIO_PIN_SET;
			}
		};
#ifdef HAL_TIM_MODULE_ENABLED
		template <TIM_HandleTypeDef *htim, uint32_t clock_frequency>
		struct TIM
		{
			static constexpr uint32_t clock_frequency_value = clock_frequency;
			static void init()
			{
				HAL_TIM_Base_Init(htim);
			}
			static TIM_HandleTypeDef *get_handle()
			{
				return htim;
			}
			static uint32_t get_clock_frequency()
			{
				return clock_frequency;
			}
			static uint32_t get_autoreload()
			{
				return __HAL_TIM_GET_AUTORELOAD(htim);
			}
			static uint32_t get_counter()
			{
				return __HAL_TIM_GET_COUNTER(htim);
			}
			static void set_prescaler(uint32_t prescaler)
			{
				__HAL_TIM_SET_PRESCALER(htim, prescaler);
			}
			static void set_autoreload(uint32_t autoreload)
			{
				__HAL_TIM_SET_AUTORELOAD(htim, autoreload);
			}
			static void set_counter(uint32_t counter)
			{
				__HAL_TIM_SET_COUNTER(htim, counter);
			}
			static void start()
			{
				HAL_TIM_Base_Start(htim);
			}
			static void start_it()
			{
				HAL_TIM_Base_Start_IT(htim);
			}
		};

		template <typename TIMtype, uint32_t channel_, uint32_t frequency_>
		struct PWM
		{
			static_assert(TIMtype::clock_frequency_value >= 1000000U, "PWM timer clock must be at least 1 MHz");
			static_assert(frequency_ > 0U && frequency_ <= 1000000U && frequency_ <= TIMtype::clock_frequency_value,
				"PWM frequency must be supported by the timer clock");

			static void set_compare(uint32_t compare)
			{
				__HAL_TIM_SET_COMPARE(TIMtype::get_handle(), channel_, compare);
			}
			static void start()
			{
				HAL_TIM_PWM_Start(TIMtype::get_handle(), channel_);
			}
			static void stop()
			{
				HAL_TIM_PWM_Stop(TIMtype::get_handle(), channel_);
			}
			static void init()
			{
				TIMtype::init();
				TIMtype::set_prescaler(TIMtype::get_clock_frequency() / 1000000U - 1U);
				TIMtype::set_autoreload(1000000U / frequency_ - 1U);
				TIMtype::set_counter(0);
				set_compare(0);
			}
			static uint32_t get_autoreload()
			{
				return TIMtype::get_autoreload();
			}
		};
#endif // HAL_TIM_MODULE_ENABLED
#ifdef HAL_I2C_MODULE_ENABLED
		template <I2C_HandleTypeDef *hi2c>
		struct I2C_bus
		{
			static constexpr I2C_HandleTypeDef *handle = hi2c;
			static void init()
			{
				HAL_I2C_Init(hi2c);
			}
			static void transmit(uint8_t address, const uint8_t *data, uint16_t size, uint32_t timeout)
			{
				HAL_I2C_Master_Transmit(hi2c, static_cast<uint16_t>(address << 1U), const_cast<uint8_t *>(data), size, timeout);
			}
			static void receive(uint8_t address, uint8_t *data, uint16_t size, uint32_t timeout)
			{
				HAL_I2C_Master_Receive(hi2c, static_cast<uint16_t>(address << 1U), data, size, timeout);
			}
		};

		template <typename bus_t, uint8_t address>
		struct I2C_device
		{
			static_assert(address < 0x80, "I2C address must be 7-bit");
			static void init() { bus_t::init(); }
			static void transmit(const uint8_t *data, uint16_t size, uint32_t timeout)
			{
				bus_t::transmit(address, data, size, timeout);
			}
			static void receive(uint8_t *data, uint16_t size, uint32_t timeout)
			{
				bus_t::receive(address, data, size, timeout);
			}
			static void mem_write(uint16_t reg, const uint8_t *data, uint16_t size, uint32_t timeout)
			{
				// Current device drivers use 8-bit register addresses; pass only the low byte to STM32 HAL.
				const uint8_t register_address = static_cast<uint8_t>(reg);
				HAL_I2C_Mem_Write(bus_t::handle, static_cast<uint16_t>(address << 1U), register_address,
					I2C_MEMADD_SIZE_8BIT,
					const_cast<uint8_t *>(data), size, timeout);
			}
			static void mem_read(uint16_t reg, uint8_t *data, uint16_t size, uint32_t timeout)
			{
				// Current device drivers use 8-bit register addresses; pass only the low byte to STM32 HAL.
				const uint8_t register_address = static_cast<uint8_t>(reg);
				HAL_I2C_Mem_Read(bus_t::handle, static_cast<uint16_t>(address << 1U), register_address,
					I2C_MEMADD_SIZE_8BIT,
					data, size, timeout);
			}
		};
#endif // HAL_I2C_MODULE_ENABLED
#ifdef HAL_UART_MODULE_ENABLED
		template <UART_HandleTypeDef *huart>
		struct UART
		{
			static void init() { HAL_UART_Init(huart); }
			static void transmit(const uint8_t *pData, uint16_t Size, uint32_t Timeout)
			{
				HAL_UART_Transmit(huart, pData, Size, Timeout);
			}
			static void receive(uint8_t *data, uint16_t size, uint32_t timeout)
			{
				HAL_UART_Receive(huart, data, size, timeout);
			}
		};
#endif
		template <uint32_t USARTx, typename config>
		struct UART_LL
		{
			static void init() { config::init(); }
			static void transmit(const uint8_t *pData, uint16_t Size, uint32_t Timeout)
			{
				(void)Timeout;
				for (uint16_t i = 0; i < Size; ++i)
				{
					LL_USART_TransmitData8(reinterpret_cast<USART_TypeDef *>(USARTx), *(pData + i));
					while (!(LL_USART_IsActiveFlag_TXE(reinterpret_cast<USART_TypeDef *>(USARTx))));
				}
			}
			static void receive(uint8_t *data, uint16_t size, uint32_t timeout)
			{
				(void)timeout;
				for (uint16_t i = 0; i < size; ++i)
				{
					while (!LL_USART_IsActiveFlag_RXNE(reinterpret_cast<USART_TypeDef *>(USARTx)));
					data[i] = LL_USART_ReceiveData8(reinterpret_cast<USART_TypeDef *>(USARTx));
				}
			}
		};
	}
}
#endif
