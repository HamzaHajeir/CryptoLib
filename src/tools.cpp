#include "CryptoLibCommon.h"

#define format_printf "%02x"

void __attribute__((weak)) printU8(const char* description, const uint8_t data[], const int size)
{
    CRYPTOLIB_PRINTF("%s\t",description);
    for (int i =0;i<size;i++)
    {
        CRYPTOLIB_PRINTF(format_printf, data[i]);
    }
    CRYPTOLIB_PRINTF("\n");
}
void __attribute__((weak)) printU8(const char *description, const AU_8_16 &data)
{
    CRYPTOLIB_PRINTF("%s\t", description);
    for (auto it = data.begin(); it != data.end(); it++)
    {
        CRYPTOLIB_PRINTF(format_printf, *it);
    }
    CRYPTOLIB_PRINTF("\n");
}
void __attribute__((weak)) printU8(const char *description, const VU_8 &data)
{
    CRYPTOLIB_PRINTF("%s\t", description);
    for (auto it = data.begin(); it != data.end(); it++)
    {
        CRYPTOLIB_PRINTF(format_printf, *it);
    }
    CRYPTOLIB_PRINTF("\n");
}