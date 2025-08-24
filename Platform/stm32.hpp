#pragma once

#ifdef __cplusplus
#include <stdint.h>

#ifdef STM32F1
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_tim.h"
#endif // STM32F1
namespace HAL
{
	namespace stm32
	{
		template <uint32_t GPIOx, uint16_t GPIO_Pin>
		struct GPIO
		/*todo:
		 *代替HAL库实现更好性能
		 *进行编译时检查
		 */
		{
			static void set()
			{
				HAL_GPIO_WritePin((GPIO_TypeDef *)GPIOx, GPIO_Pin, GPIO_PIN_SET);
			}
			static void clear()
			{
				HAL_GPIO_WritePin((GPIO_TypeDef *)GPIOx, GPIO_Pin, GPIO_PIN_RESET);
			}
			static void toggle()
			{
				HAL_GPIO_TogglePin((GPIO_TypeDef *)GPIOx, GPIO_Pin);
			}
			static bool read()
			{
				return (bool)HAL_GPIO_ReadPin((GPIO_TypeDef *)GPIOx, GPIO_Pin);
			}
		};

		template <TIM_HandleTypeDef *htim, uint32_t clock_frequency>
		struct TIM
		{
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
			static void set_compare(uint32_t compare)
			{
				__HAL_TIM_SET_COMPARE(TIMtype::get_handle(), channel_, compare);
			}
			static void set_frequency(uint32_t frequency)
			{
				if (frequency > 0 && frequency <= 1000000)
				{
					/*注意启用预装载*/
					__HAL_TIM_SET_PRESCALER(TIMtype::get_handle(), (TIMtype::get_clock_frequency() / 1000000.0f) - 1);
					__HAL_TIM_SET_AUTORELOAD(TIMtype::get_handle(), 1000000.0f / frequency - 1);
				}
			}
			static void start()
			{
				HAL_TIM_PWM_Start(TIMtype::get_handle(), channel_);
			}
			static void init()
			{
				static_assert(!(frequency_ == 0 || frequency_ > 1000000), "PWM frequency must be in range 1-1000000 Hz");
				TIMtype::set_prescaler(TIMtype::get_clock_frequency() / 1000000.0f - 1);
				TIMtype::set_autoreload(1000000.0f / frequency_ - 1);
				TIMtype::set_counter(0);
				set_compare(0);
				start();
			}
			static uint32_t get_channel()
			{
				return channel_;
			}
			static uint32_t get_autoreload()
			{
				return TIMtype::get_autoreload();
			}
		};
	}
}
#endif