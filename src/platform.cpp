
#include "platform.h"

#ifdef STM32F0
#include "stm32f0xx.h"
#endif
#ifdef STM32F1
#include "stm32f1xx.h"
#endif
#ifdef STM32F2
#include "stm32f2xx.h"
#endif
#ifdef STM32F3
#include "stm32f3xx.h"
#endif
#ifdef STM32F4
#include "stm32f4xx.h"
#endif
#ifdef STM32F7
#include "stm32f7xx.h"
#endif


uint32_t __attribute__((weak)) getRandomNumber(int seed)
{
#ifdef ARDUINO
const uint32_t MAX_VALUE_32_BITS = pow(2, 32);

#ifdef ARDUINO_ARCH_ESP8266 || defined(ARDUINO_ARCH_ESP32)
    randomSeed(getCycleCount());
#else
    randomSeed(micros());
#endif
    return random(MAX_VALUE_32_BITS);

#else
    static uint32_t rand;
    rand += getCycleCount();
    rand += 351968432;
    rand *= 32938911;
    rand *= seed;
    return rand;
#endif
}

#ifdef USE_HAL_DRIVER
void cycleCounterInit()
{        
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}
#endif