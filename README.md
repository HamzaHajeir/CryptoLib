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
## How to contribute?

The current *alpha* design for the library is:
- Maintaining Crypto.h to have all encryption/decryption interfaces inside `CryptoLib` namespace.
- Uses separate files for separate functionalities: cmac, random, platform, Crypto ... 
Any suggestion for a better heirarcy, please share with us.
- to use `std::array<>` and `std::vector<>` in the interfaces. 
- Uses `platform.h` and `platform.cpp` for portability of the library to other hardware platforms.
