
#include "platform.h"

const uint32_t MAX_VALUE_32_BITS = pow(2, 32);


uint32_t __attribute__((weak)) getRandomNumber(int seed)
{
#ifdef ARDUINO

    randomSeed(getCycleCount());
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

#ifdef CUBEIDE
void cycleCounterInit()
{        
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}
#endif