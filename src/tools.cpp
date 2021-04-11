#include "CryptoLibCommon.h"

#define format_printf "%02x"

void __attribute__((weak)) printU8(const char* description, const uint8_t data[], const int size)
{
    PRINTF("%s\t",description);
    for (int i =0;i<size;i++)
    {
        PRINTF(format_printf, data[i]);
    }
    PRINTF("\n");
}
void __attribute__((weak)) printU8(const char *description, const AU_8_16 &data)
{
    PRINTF("%s\t", description);
    for (auto it = data.begin(); it != data.end(); it++)
    {
        PRINTF(format_printf, *it);
    }
    PRINTF("\n");
}
void __attribute__((weak)) printU8(const char *description, const VU_8 &data)
{
    PRINTF("%s\t", description);
    for (auto it = data.begin(); it != data.end(); it++)
    {
        PRINTF(format_printf, *it);
    }
    PRINTF("\n");
}