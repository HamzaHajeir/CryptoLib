#pragma once
#include <stdint.h>

uint32_t getRandomNumber(int seed = 1);  //awful and very weak random number!
#ifdef ARDUINO
#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP8266 || defined(ARDUINO_ARCH_ESP32)
#include <ESP.h>

#define Crypto_getClock() ESP.getCycleCount()
#define PLATFORM_PRINTF(x,...) Serial.printf((x), ##__VA_ARGS__)
#endif  //ARDUINO_ARCH_ESP8266 || defined(ARDUINO_ARCH_ESP32)

/* 
    If you'd prefer using your own platform's clock cycle instead of Arduino's macros(),
    Define your platform's clockCycle here
*/

#elif defined(USE_HAL_DRIVER)
#ifdef __cplusplus
extern "C"{
#endif
void cycleCounterInit();
#define Crypto_getClock() DWT->CYCCNT
#define PLATFORM_PRINTF(x,...) printf((x), ##__VA_ARGS__)
#ifdef __cplusplus
}
#else      //Define your own!
#error "CriptoLib: Define your platform specific functions"
#define Crypto_getClock() DWT->CYCCNT
#define PLATFORM_PRINTF(x,...) printf((x), ##__VA_ARGS__)
#endif
