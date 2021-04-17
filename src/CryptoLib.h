#pragma once

#include "CryptoLibCommon.h"
#include <mbedtls/config.h>
#include <mbedtls/aes.h>
// #include "mbedtls/entropy.h"        //MBEDTLS_ENTROPY_C
// #include "mbedtls/ctr_drbg.h"   //MBEDTLS_CTR_DRBG_C
#include "CMAC.h"
#include "tools.h"


/** 
    encryption routine : 
        1- encrypt , hold iv
        2- cmac and append
        3- prepend iv
**/

/**
    decryption routine : 
        1- validateMIC
        2- removeMIC
        3- parse_and_decrypt
**/

//encrypt_cbc(vector &plainText,iv) Encrypt with variable input size + one IV.
/* 
    1- Calculates size
    2- Pads the message if it doesn't fill 128 bits block as the size.
    3- Calls encryption routine, Witch takes care of chaining if the message is greater than 16-bytes. 

    How to use : 
        * Prepare the message in a vector<uint8_t> data type.
        * Generate the random IV inside a std::array<uint8_t,16>.
        * Pass the message & IV & the Key for the encryption.
*/
namespace CryptoLib
{
    int encrypt_cbc(VU_8 &plainText, AU_8_16 &, const AU_8_16 &);

    /* 
    Generates Random IV to be used in the encrypting system.    
    This depends on the MCU clock Timer for random-ness. It depends on randomNumberGen files.

    IV is used at the encryption/transmitting side.

    Returns a Random IV.

    How to use :
        * Make a buffer of std::array<uint8_t,16>
        * Pass it to the function call.

 */
    int generateIV(AU_8_16 &iv);

    //generate_iv_and_encrypt(vector &plainText,unsigned char* iv)
    /* 
    1- generates iv
    2- passes to encrypt_cbc
    3- returns cipher to the same Inputted message and the used iv (Not the resulting IV after CBC block encryption)

    How to use :
        * create iv buffer (std::array<uint8_t,16> : uint8_t type with 16 bytes long
        * pass plainText vector and iv buffer
*/
    int generate_iv_and_encrypt(VU_8 &plainText, AU_8_16 &iv, const AU_8_16 &key);


    /* 
    A simple tool to prepend the vector message with a given IV.

    Returns a prepended message by a given IV.

    How to Use : 
        * Prepare the message (usually encrypted message)
        * Pass the used IV in the encryption routine.
*/
    void prependIV(VU_8 &cipherText, AU_8_16 &iv);

    //encrypt_and_cmac(vector &plainText)
    /* 
    Takes the plain text, Converts it to complete block of ciphertext, Containing the IV,EncryptedText,CMAC
    Which takes the following route :
    1- generate_iv_and_encrypt
    2- cmac_message
    3- prependIV

    It does CMAC the encrypted text with no IV prepended, Then prepends with IV.
    It's the One-Step method for encryption side.

    Returns a complete block of ciphertext that contains IV|EncryptedText|CMAC.
    Returns it to the SAME message source
    Returns 0 on Success.
    non-zero for mbedtls error codes.

    How to use : 
        * Prepare your message inside vector<uint8_t>
        * Pass the message alongwith the used Key.
*/
    int encrypt_and_cmac(VU_8 &plainText, const AU_8_16 &);

    //decrypt_cbc(vector &encryptedText,iv)
    /* 
    Decrypt the message using extracted IV and Key.
    1- decrypts using key and iv
    2- returns the message to the source.

    How to use : 
        * Parse the received ciphertext To 2 main entities at least : Encrypted Text and IV (excluding CMAC)
        * Pass the encryptedText, IV , and used Key to decrypt the message.
*/
    int decrypt_cbc(VU_8 &encryptedText, AU_8_16 &iv, const AU_8_16 &_key);

    //parse_and_decrypt(vector &ciphertext) Pass all ciphertext along with IV
    /* 
    Decrypts the ciphertext containing IV and EncryptedText. If there's a MIC should be removed first.

    Returns decrypted plain text to the same cipher source vector.

    How to use :
        * Receive the ciphertext and remove MIC if any (After validation if needed)
        * Pass the ciphertext and the used Key for decryption.

*/
    int parse_and_decrypt(VU_8 &ciphertext, const AU_8_16 &);
} //namespace CryptoLib