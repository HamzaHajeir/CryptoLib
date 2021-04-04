#pragma once

#include <array>
#include <vector>
#include "platform.h"

#define DEBUGGING_MODE

using VU_8 = std::vector<uint8_t>;
using AU_8_16 = std::array<uint8_t,16>;

#ifdef DEBUGGING_MODE
#define PRINTF(x,...)   PLATFORM_PRINTF((x), ##__VA_ARGS__)
#else
#define PRINTF(x,...)
#endif
