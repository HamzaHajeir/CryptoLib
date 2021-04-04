# CryptoLib

A C++ Wrapper to mbedtls encryption modes. Starts with minimal implementation supports  the following:
- CBC Mode Encryption.
- CMAC as message authentication.

This library interfaces to mbedtls functions, in a C++ way: using std::array and std::vector for keys and messages.

Currently, The supported key size is 128-bits. (Any idea to abstract it?)

CBC and CMAC are used altogether for LoRaWAN protocol.
These modes (CBC and CMAC) are developed for securing a custom protocol. 

The aim for sharing this library beside saving time, is to help C++ programmers from stopping C-style code from (bleeding into) their codes.

-------
## How to use it?
1. You should install [mbedtls](https://github.com/ARMmbed/mbedtls/releases/tag/v2.23.0) library.
2. Replace the default `\include\mbedtls\config.h` file with provided [config.h](config.h) file. 

It currently supported on ESP8266, ESP32, And STM32. If you have different platform, It's not hard to add your customization to `platform.h` and `platform.cpp`, which should include:
- PRINTF definition support (if used debugging)
- getCycleCount()  definition to a high clock speed function, used as entropy source.  
You can define your own entropy source by defining `uint32_t getRandomNumber(int seed)` into your skecth

## STM32CubeIDE users:
For the library to work properly, It's important to activate the Cycle Counter `DWT` by calling `cycleCounterInit()` at the beginning of your skecth.

-------
## How to contribute?

The current *alpha* design for the library is:
- Maintaining Crypto.h to have all encryption/decryption interfaces inside `CryptoLib` namespace.
- Uses separate files for separate functionalities: cmac, random, platform, Crypto ... 
Any suggestion for a better heirarcy, please share with us.
- to use `std::array<>` and `std::vector<>` in the interfaces. 
- Uses `platform.h` and `platform.cpp` for portability of the library to other hardware platforms.
