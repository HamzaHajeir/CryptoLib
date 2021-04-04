#ifdef ARDUINO
#include <Arduino.h>
#endif
#include <algorithm>

#include "common.h"
#include "random.h"
#include "tools.h"
#include "platform.h"




void __attribute__((weak)) getRandomIV(AU_8_16& iv)
{

    uint8_t outputBuffer[16];

    int ret;
    for (int i = 0; i < 4; i++)
    {
        
        uint32_t randomValue = getRandomNumber();
        PRINTF("%04x ", randomValue);
        for (int j = 0; j < 4; j++)
        {
            outputBuffer[i * 4 + j] = randomValue >> (8 * j) & 0xff;
        }
    }

    std::copy_n(outputBuffer, 16, iv.begin());
}