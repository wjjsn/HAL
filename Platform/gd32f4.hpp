#pragma once

#ifdef __cplusplus
#include <cstdint>
#include <concepts>
extern "C"
{
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_usart.h"
#include "gd32f4xx_i2c.h"
#include "gd32f4xx_spi.h"
#include "gd32f4xx_timer.h"
#include "gd32f4xx_misc.h"
}
namespace HAL
{
	namespace gd32f4
	{

		template <uint32_t PERIPH>
		struct RCU_periph
		{
			static constexpr rcu_periph_enum periph = []
			{
				if constexpr (PERIPH == GPIOA)
					return RCU_GPIOA;
				else if constexpr (PERIPH == GPIOB)
					return RCU_GPIOB;
				else if constexpr (PERIPH == GPIOC)
					return RCU_GPIOC;
				else if constexpr (PERIPH == GPIOD)
					return RCU_GPIOD;
				else if constexpr (PERIPH == GPIOE)
					return RCU_GPIOE;
				else if constexpr (PERIPH == GPIOF)
					return RCU_GPIOF;
				else if constexpr (PERIPH == GPIOG)
					return RCU_GPIOG;
				else if constexpr (PERIPH == GPIOH)
					return RCU_GPIOH;
				else if constexpr (PERIPH == GPIOI)
					return RCU_GPIOI;
				else if constexpr (PERIPH == TIMER0)
					return RCU_TIMER0;
				else if constexpr (PERIPH == TIMER1)
					return RCU_TIMER1;
				else if constexpr (PERIPH == TIMER2)
					return RCU_TIMER2;
				else if constexpr (PERIPH == TIMER3)
					return RCU_TIMER3;
				else if constexpr (PERIPH == TIMER4)
					return RCU_TIMER4;
				else if constexpr (PERIPH == TIMER5)
					return RCU_TIMER5;
				else if constexpr (PERIPH == TIMER6)
					return RCU_TIMER6;
				else if constexpr (PERIPH == TIMER7)
					return RCU_TIMER7;
				else if constexpr (PERIPH == TIMER8)
					return RCU_TIMER8;
				else if constexpr (PERIPH == TIMER9)
					return RCU_TIMER9;
				else if constexpr (PERIPH == TIMER10)
					return RCU_TIMER10;
				else if constexpr (PERIPH == TIMER11)
					return RCU_TIMER11;
				else if constexpr (PERIPH == TIMER12)
					return RCU_TIMER12;
				else if constexpr (PERIPH == TIMER13)
					return RCU_TIMER13;
				else if constexpr (PERIPH == USART0)
					return RCU_USART0;
				else if constexpr (PERIPH == USART1)
					return RCU_USART1;
				else if constexpr (PERIPH == USART2)
					return RCU_USART2;
				else if constexpr (PERIPH == USART5)
					return RCU_USART5;
				else if constexpr (PERIPH == UART3)
					return RCU_UART3;
				else if constexpr (PERIPH == UART4)
					return RCU_UART4;
				else if constexpr (PERIPH == UART6)
					return RCU_UART6;
				else if constexpr (PERIPH == UART7)
					return RCU_UART7;
				else if constexpr (PERIPH == I2C0)
					return RCU_I2C0;
				else if constexpr (PERIPH == I2C1)
					return RCU_I2C1;
				else if constexpr (PERIPH == I2C2)
					return RCU_I2C2;
				else if constexpr (PERIPH == SPI0)
					return RCU_SPI0;
				else if constexpr (PERIPH == SPI1)
					return RCU_SPI1;
				else if constexpr (PERIPH == SPI2)
					return RCU_SPI2;
				else if constexpr (PERIPH == SPI3)
					return RCU_SPI3;
				else if constexpr (PERIPH == SPI4)
					return RCU_SPI4;
				else if constexpr (PERIPH == SPI5)
					return RCU_SPI5;
				else
					return []<bool b = false>()
					{ static_assert(b, "Unsupported PERIPH"); return 0; }();
			}();
		};

		template <uint8_t OTYPE, uint32_t SPEED, bit_status VAL>
		struct OutputConfig
		{
			static constexpr uint8_t otype	= OTYPE;
			static constexpr uint32_t speed = SPEED;
			static constexpr bit_status val = VAL;
		};

		template <uint32_t AF_NUM>
		struct AFConfig
		{
			static constexpr uint32_t af_num = AF_NUM;
		};

		template <uint32_t GPIOx, uint16_t Pin, uint32_t Mode, uint32_t PULL, typename config>
		struct GPIO
		{
			static_assert((false), "fallback");
		};

		template <uint32_t GPIOx, uint16_t Pin, uint32_t PULL, uint8_t OTYPE, uint32_t SPEED, bit_status VAL>
		struct GPIO<GPIOx, Pin, GPIO_MODE_OUTPUT, PULL, OutputConfig<OTYPE, SPEED, VAL>>
		{
			static void init()
			{
				rcu_periph_clock_enable(RCU_periph<GPIOx>::periph);
				gpio_mode_set(GPIOx, GPIO_MODE_OUTPUT, PULL, Pin);
				gpio_output_options_set(GPIOx, OTYPE, SPEED, Pin);
				gpio_bit_write(GPIOx, Pin, VAL);
			}
			static void set()
			{
				gpio_bit_write(GPIOx, Pin, SET);
			}
			static void clear()
			{
				gpio_bit_reset(GPIOx, Pin);
			}
			static void toggle()
			{
				gpio_bit_toggle(GPIOx, Pin);
			}
			static bool read()
			{
				return gpio_input_bit_get(GPIOx, Pin) == SET;
			}
		};

		template <uint32_t GPIOx, uint16_t Pin, uint32_t PULL, uint32_t AF_NUM>
		struct GPIO<GPIOx, Pin, GPIO_MODE_AF, PULL, AFConfig<AF_NUM>>
		{
			using af_config = AFConfig<AF_NUM>;

			static void init()
			{
				rcu_periph_clock_enable(RCU_periph<GPIOx>::periph);
				gpio_mode_set(GPIOx, GPIO_MODE_AF, PULL, Pin);
				gpio_af_set(GPIOx, AF_NUM, Pin);
			}
			static void set()
			{
				gpio_bit_write(GPIOx, Pin, SET);
			}
			static void clear()
			{
				gpio_bit_reset(GPIOx, Pin);
			}
			static void toggle()
			{
				gpio_bit_toggle(GPIOx, Pin);
			}
			static bool read()
			{
				return gpio_input_bit_get(GPIOx, Pin) == SET;
			}
		};

		template <uint32_t GPIOx, uint16_t Pin, uint32_t PULL>
		struct GPIO<GPIOx, Pin, GPIO_MODE_INPUT, PULL, void>
		{
			static void init()
			{
				rcu_periph_clock_enable(RCU_periph<GPIOx>::periph);
				gpio_mode_set(GPIOx, GPIO_MODE_INPUT, PULL, Pin);
			}
			static bool read()
			{
				return gpio_input_bit_get(GPIOx, Pin) == SET;
			}
		};

		template <typename GPIO_SDA, typename GPIO_SCL, uint32_t I2Cx, uint32_t clkspeed, uint8_t slave_address, uint8_t own_address7 = 0x00>
			requires(
				GPIO_SDA::af_config::af_num == GPIO_AF_4 &&
				GPIO_SCL::af_config::af_num == GPIO_AF_4)
		struct I2C_device_7bits
		{
			static void init()
			{
				GPIO_SDA::init();
				GPIO_SCL::init();

				rcu_periph_clock_enable(RCU_periph<I2Cx>::periph);

				i2c_clock_config(I2Cx, clkspeed, I2C_DTCY_2);
				i2c_mode_addr_config(I2Cx, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, own_address7);
				i2c_enable(I2Cx);
				i2c_ack_config(I2Cx, I2C_ACK_ENABLE);
			}

			static void transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout)
			{
				(void)Timeout;
				while (i2c_flag_get(I2Cx, I2C_FLAG_I2CBSY));
				i2c_start_on_bus(I2Cx);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_SBSEND));
				i2c_master_addressing(I2Cx, slave_address, I2C_TRANSMITTER);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_ADDSEND));
				i2c_flag_clear(I2Cx, I2C_FLAG_ADDSEND);
				for (uint16_t i = 0; i < Size; i++)
				{
					while (!i2c_flag_get(I2Cx, I2C_FLAG_TBE));
					i2c_data_transmit(I2Cx, *(pData + i));
				}
				while (!i2c_flag_get(I2Cx, I2C_FLAG_BTC));
				i2c_stop_on_bus(I2Cx);
				while (I2C_CTL0(I2Cx) & I2C_CTL0_STOP);
			}

			static void receive(uint8_t *pData, uint16_t Size, uint32_t Timeout)
			{
				(void)Timeout;
				while (i2c_flag_get(I2Cx, I2C_FLAG_I2CBSY));
				i2c_start_on_bus(I2Cx);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_SBSEND));
				i2c_master_addressing(I2Cx, slave_address, I2C_RECEIVER);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_ADDSEND));
				i2c_flag_clear(I2Cx, I2C_FLAG_ADDSEND);
				if (Size == 1)
				{
					i2c_ack_config(I2Cx, I2C_ACK_DISABLE);
					while (!i2c_flag_get(I2Cx, I2C_FLAG_RBNE));
					*pData = i2c_data_receive(I2Cx);
					i2c_stop_on_bus(I2Cx);
				}
				else
				{
					while (!i2c_flag_get(I2Cx, I2C_FLAG_RBNE));
					*pData = i2c_data_receive(I2Cx);
					for (uint16_t i = 1; i < Size - 1; i++)
					{
						i2c_ack_config(I2Cx, I2C_ACK_ENABLE);
						while (!i2c_flag_get(I2Cx, I2C_FLAG_RBNE));
						*(pData + i) = i2c_data_receive(I2Cx);
					}
					i2c_ack_config(I2Cx, I2C_ACK_DISABLE);
					while (!i2c_flag_get(I2Cx, I2C_FLAG_RBNE));
					*(pData + Size - 1) = i2c_data_receive(I2Cx);
					i2c_stop_on_bus(I2Cx);
				}
				while (I2C_CTL0(I2Cx) & I2C_CTL0_STOP);
			}

			static void mem_write(uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
			{
				(void)Timeout;
				while (i2c_flag_get(I2Cx, I2C_FLAG_I2CBSY));
				i2c_start_on_bus(I2Cx);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_SBSEND));
				i2c_master_addressing(I2Cx, slave_address, I2C_TRANSMITTER);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_ADDSEND));
				i2c_flag_clear(I2Cx, I2C_FLAG_ADDSEND);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_TBE));
				i2c_data_transmit(I2Cx, MemAddress);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_BTC));
				for (uint16_t i = 0; i < Size; i++)
				{
					while (!i2c_flag_get(I2Cx, I2C_FLAG_TBE));
					i2c_data_transmit(I2Cx, *(pData + i));
				}
				while (!i2c_flag_get(I2Cx, I2C_FLAG_BTC));
				i2c_stop_on_bus(I2Cx);
				while (I2C_CTL0(I2Cx) & I2C_CTL0_STOP);
			}

			static void mem_read(uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
			{
				(void)Timeout;
				while (i2c_flag_get(I2Cx, I2C_FLAG_I2CBSY));
				i2c_start_on_bus(I2Cx);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_SBSEND));
				i2c_master_addressing(I2Cx, slave_address, I2C_TRANSMITTER);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_ADDSEND));
				i2c_flag_clear(I2Cx, I2C_FLAG_ADDSEND);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_TBE));
				i2c_data_transmit(I2Cx, MemAddress);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_BTC));

				i2c_start_on_bus(I2Cx);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_SBSEND));
				i2c_master_addressing(I2Cx, slave_address, I2C_RECEIVER);
				while (!i2c_flag_get(I2Cx, I2C_FLAG_ADDSEND));
				i2c_flag_clear(I2Cx, I2C_FLAG_ADDSEND);

				if (Size == 1)
				{
					i2c_ack_config(I2Cx, I2C_ACK_DISABLE);
					while (!i2c_flag_get(I2Cx, I2C_FLAG_RBNE));
					*pData = i2c_data_receive(I2Cx);
					i2c_stop_on_bus(I2Cx);
				}
				else
				{
					while (!i2c_flag_get(I2Cx, I2C_FLAG_RBNE));
					*pData = i2c_data_receive(I2Cx);
					for (uint16_t i = 1; i < Size - 1; i++)
					{
						i2c_ack_config(I2Cx, I2C_ACK_ENABLE);
						while (!i2c_flag_get(I2Cx, I2C_FLAG_RBNE));
						*(pData + i) = i2c_data_receive(I2Cx);
					}
					i2c_ack_config(I2Cx, I2C_ACK_DISABLE);
					while (!i2c_flag_get(I2Cx, I2C_FLAG_RBNE));
					*(pData + Size - 1) = i2c_data_receive(I2Cx);
					i2c_stop_on_bus(I2Cx);
				}
				while (I2C_CTL0(I2Cx) & I2C_CTL0_STOP);
			}
		};

		template <typename GPIO_TX, typename GPIO_RX, uint32_t USARTx, uint32_t baudval,
				  uint32_t paritycfg = USART_PM_NONE, uint32_t wlen = USART_WL_8BIT, uint32_t stblen = USART_STB_1BIT>
			requires(
				(GPIO_TX::af_config::af_num == GPIO_AF_7 || GPIO_TX::af_config::af_num == GPIO_AF_8) &&
				(GPIO_RX::af_config::af_num == GPIO_AF_7 || GPIO_RX::af_config::af_num == GPIO_AF_8))
		struct USART
		{
			static constexpr IRQn_Type get_irqn()
			{
				if constexpr (USARTx == USART0)
					return USART0_IRQn;
				else if constexpr (USARTx == USART1)
					return USART1_IRQn;
				else if constexpr (USARTx == USART2)
					return USART2_IRQn;
				else if constexpr (USARTx == USART5)
					return USART5_IRQn;
				else if constexpr (USARTx == UART3)
					return UART3_IRQn;
				else if constexpr (USARTx == UART4)
					return UART4_IRQn;
				else if constexpr (USARTx == UART6)
					return UART6_IRQn;
				else if constexpr (USARTx == UART7)
					return UART7_IRQn;
				else
					return USART0_IRQn;
			}

			static void init()
			{
				GPIO_TX::init();
				GPIO_RX::init();

				rcu_periph_clock_enable(RCU_periph<USARTx>::periph);

				usart_deinit(USARTx);
				usart_baudrate_set(USARTx, baudval);
				usart_parity_config(USARTx, paritycfg);
				usart_word_length_set(USARTx, wlen);
				usart_stop_bit_set(USARTx, stblen);
				usart_transmit_config(USARTx, USART_TRANSMIT_ENABLE);
				usart_receive_config(USARTx, USART_RECEIVE_ENABLE);
				usart_enable(USARTx);
			}

			static void transmit(const uint8_t *pData, uint16_t Size, uint32_t Timeout)
			{
				(void)Timeout;
				for (uint16_t i = 0; i < Size; i++)
				{
					while (usart_flag_get(USARTx, USART_FLAG_TBE) == RESET);
					usart_data_transmit(USARTx, *(pData + i));
				}
				while (usart_flag_get(USARTx, USART_FLAG_TC) == RESET);
			}

			static void enable_it(uint8_t priority, uint8_t sub_priority)
			{
				nvic_irq_enable(get_irqn(), priority, sub_priority);
				usart_interrupt_enable(USARTx, USART_INT_RBNE);
			}
		};

		template <typename GPIO_MOSI, typename GPIO_MISO, typename GPIO_SCLK, uint32_t SPIx,
				  uint32_t prescaler = SPI_PSC_64, uint32_t clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE,
				  uint32_t device_mode = SPI_MASTER, uint32_t nss = SPI_NSS_SOFT>
			requires(
				GPIO_MOSI::af_config::af_num == GPIO_AF_5 &&
				GPIO_MISO::af_config::af_num == GPIO_AF_5 &&
				GPIO_SCLK::af_config::af_num == GPIO_AF_5)
		struct SPI
		{
			static void init()
			{
				GPIO_MOSI::init();
				GPIO_MISO::init();
				GPIO_SCLK::init();

				rcu_periph_clock_enable(RCU_periph<SPIx>::periph);

				spi_parameter_struct spi_init_struct = {
					device_mode,
					SPI_TRANSMODE_FULLDUPLEX,
					SPI_FRAMESIZE_8BIT,
					nss,
					SPI_ENDIAN_MSB,
					clock_polarity_phase,
					prescaler};
				spi_init(SPIx, &spi_init_struct);
				spi_enable(SPIx);
			}

			static uint8_t send_rec_byte(uint8_t byte)
			{
				while (spi_i2s_flag_get(SPIx, SPI_FLAG_TBE) == RESET);
				spi_i2s_data_transmit(SPIx, byte);
				while (spi_i2s_flag_get(SPIx, SPI_FLAG_RBNE) == RESET);
				return spi_i2s_data_receive(SPIx);
			}

			static void send_bytes(const uint8_t *pSend, uint16_t len)
			{
				for (uint16_t i = 0; i < len; i++)
				{
					send_rec_byte(*pSend++);
				}
			}

			static void rec_bytes(uint8_t dummy_byte, uint8_t *pRec, uint16_t len)
			{
				for (uint16_t i = 0; i < len; i++)
				{
					*pRec++ = send_rec_byte(dummy_byte);
				}
			}

			static void send_rec_bytes(const uint8_t *pSend, uint8_t *pRec, uint16_t len)
			{
				for (uint16_t i = 0; i < len; i++)
				{
					*pRec++ = send_rec_byte(*pSend++);
				}
			}
		};

		template <uint32_t TIMx, uint32_t clock_frequency, uint32_t psc_mul = RCU_TIMER_PSC_MUL4>
		struct TIM
		{
			static constexpr uint32_t tim_base = TIMx;

			static void init(uint16_t prescaler, uint32_t autoreload)
			{
				rcu_periph_clock_enable(RCU_periph<TIMx>::periph);
				rcu_timer_clock_prescaler_config(psc_mul);

				timer_parameter_struct param = {
					prescaler,
					TIMER_COUNTER_EDGE,
					TIMER_COUNTER_UP,
					TIMER_CKDIV_DIV1,
					autoreload,
					0};
				timer_init(TIMx, &param);
				timer_enable(TIMx);
			}

			static uint32_t get_handle()
			{
				return TIMx;
			}
			static uint32_t get_clock_frequency()
			{
				return clock_frequency;
			}
			static uint32_t get_counter()
			{
				return timer_counter_read(TIMx);
			}
			static void set_counter(uint32_t counter)
			{
				timer_counter_value_config(TIMx, counter);
			}
			static void start()
			{
				timer_enable(TIMx);
			}
			static void stop()
			{
				timer_disable(TIMx);
			}
		};

		template <typename TIMtype, uint32_t channel_, uint32_t frequency_>
		struct PWM
		{
			static void init()
			{
				TIMtype::init(
					TIMtype::get_clock_frequency() / 1000000.0f - 1,
					1000000.0f / frequency_ - 1);

				timer_oc_parameter_struct oc_param = {
					TIMER_CCX_ENABLE,
					TIMER_CCXN_DISABLE,
					TIMER_OC_POLARITY_HIGH,
					TIMER_OCN_POLARITY_HIGH,
					TIMER_OC_IDLE_STATE_LOW,
					TIMER_OCN_IDLE_STATE_LOW};
				timer_channel_output_config(TIMtype::get_handle(), channel_, &oc_param);
				timer_channel_output_mode_config(TIMtype::get_handle(), channel_, TIMER_OC_MODE_PWM0);
				timer_channel_output_shadow_config(TIMtype::get_handle(), channel_, TIMER_OC_SHADOW_DISABLE);
			}

			static void set_compare(uint32_t compare)
			{
				timer_channel_output_pulse_value_config(TIMtype::get_handle(), channel_, compare);
			}

			static void start()
			{
				timer_channel_output_state_config(TIMtype::get_handle(), channel_, TIMER_CCX_ENABLE);
			}

			static void stop()
			{
				timer_channel_output_state_config(TIMtype::get_handle(), channel_, 0);
			}
		};
	}
}
#endif // __cplusplus
