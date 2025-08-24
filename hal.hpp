#pragma once

/*Choice Platform*/
// #define STM32F1
// #define MSPM0



#if defined(STM32F1) || defined(STM32F4)
#include "Platform/stm32.hpp"
#endif

#if defined(MSPM0)
#include "Platform/mspm0.hpp"
#endif