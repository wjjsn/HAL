#pragma once
#if 1
#ifdef __cplusplus
#include <stdint.h>

#include "ti_msp_dl_config.h"

namespace HAL
{
	namespace mspm0
	{
		template <uint32_t GPIOx, uint32_t GPIO_Pin, typename config>
		struct GPIO
		/*todo:
		 *代替HAL库实现更好性能
		 *进行编译时检查
		 */
		{
			static void init() { config::init(); }
			static void set()
			{
				DL_GPIO_setPins(reinterpret_cast<GPIO_Regs *>(GPIOx), GPIO_Pin);
			}
			static void clear()
			{
				DL_GPIO_clearPins(reinterpret_cast<GPIO_Regs *>(GPIOx), GPIO_Pin);
			}
			static void toggle()
			{
				DL_GPIO_togglePins(reinterpret_cast<GPIO_Regs *>(GPIOx), GPIO_Pin);
			}
			static bool read()
			{
				return DL_GPIO_readPins(reinterpret_cast<GPIO_Regs *>(GPIOx), GPIO_Pin) != 0U;
			}
		};
#ifdef GPIOA_BASE
		template <uint32_t GPIO_Pin, typename config>
		using PA = GPIO<GPIOA_BASE, GPIO_Pin, config>;
#endif // GPIOA_BASE
#ifdef GPIOB_BASE
		template <uint32_t GPIO_Pin, typename config>
		using PB = GPIO<GPIOB_BASE, GPIO_Pin, config>;
#endif // GPIOB_BASE

		template <uint32_t tim_base, uint32_t clock_frequency, typename config>
		struct TIM
		{
			static constexpr uint32_t clock_frequency_value = clock_frequency;
			static void init() { config::init(); }
			static uint32_t get_handle()
			{
				return tim_base;
			}
			static uint32_t get_clock_frequency()
			{
				return clock_frequency;
			}
			static uint32_t get_autoreload()
			{
				return DL_Timer_getLoadValue(reinterpret_cast<GPTIMER_Regs *>(tim_base));
			}
			static uint32_t get_counter()
			{
				return DL_Timer_getTimerCount(reinterpret_cast<GPTIMER_Regs *>(tim_base));
			}
			static void set_prescaler(uint32_t prescaler)
			{
				reinterpret_cast<GPTIMER_Regs *>(tim_base)->COMMONREGS.CPS = prescaler;
			}
			static void set_autoreload(uint32_t autoreload)
			{
				DL_Timer_setLoadValue(reinterpret_cast<GPTIMER_Regs *>(tim_base), autoreload);
			}
			static void set_counter(uint32_t counter)
			{
				DL_Timer_setTimerCount(reinterpret_cast<GPTIMER_Regs *>(tim_base), counter);
			}
			static void start()
			{
				DL_Timer_startCounter(reinterpret_cast<GPTIMER_Regs *>(tim_base));
			}
			static void start_it()
			{
				config::start_it();
			}
		};

		template <typename TIMtype, DL_TIMER_CC_INDEX channel_, uint32_t frequency_>
		struct PWM
		{
			static_assert(TIMtype::clock_frequency_value >= 1000000U, "PWM timer clock must be at least 1 MHz");
			static_assert(frequency_ > 0U && frequency_ <= 1000000U && frequency_ <= TIMtype::clock_frequency_value,
				"PWM frequency must be supported by the timer clock");
			static void set_compare(uint32_t compare)
			{
				// __HAL_TIM_SET_COMPARE(TIMtype::get_handle(), channel_, compare);
				DL_Timer_setCaptureCompareValue(reinterpret_cast<GPTIMER_Regs *>(TIMtype::get_handle()), compare, channel_);
			}
			static void start()
			{
				TIMtype::start();
			}
			static void stop()
			{
				DL_Timer_stopCounter(reinterpret_cast<GPTIMER_Regs *>(TIMtype::get_handle()));
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
	}
}
#endif

#endif
