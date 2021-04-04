#pragma once
#include <stdint.h>

uint32_t getRandomNumber(int seed = 1);  //awful and very weak random number!

#ifdef ARDUINO_ARCH_ESP8266 || defined(ARDUINO_ARCH_ESP32)
#include <Arduino.h>
#include <ESP.h>

#define getCycleCount() ESP.getCycleCount()
#define PLATFORM_PRINTF(x,...) Serial.printf((x), ##__VA_ARGS__)

#else       //Define your own!

#define getCycleCount() ESP.getCycleCount()
#define PLATFORM_PRINTF(x,...) Serial.printf((x), ##__VA_ARGS__)
#endif
