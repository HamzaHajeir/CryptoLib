#include "Crypto.h"
#include "mbedtls/entropy_poll.h"
#include <algorithm>
#include <string>
#include "tools.h"
#include "random.h"

mbedtls_aes_context aes;
using namespace CryptoLib; //For TEST functions




//encrypt_cbc(vector &plainText,iv) Encrypt with variable input size + one IV.
/* 
    1- calc size
    2- pad plainText
    3- encrypt
*/
int CryptoLib::encrypt_cbc(VU_8 &plainText, AU_8_16 &iv, const AU_8_16 &passedKey)
{
    AU_8_16 iv_copy = iv;
    printU8("passed iv= ", iv);

    printU8("PassedKey:", passedKey);

    int ret;
    size_t input_len = plainText.size();

    int padMessageLength = 16 - (input_len % 16);
    PRINTF("Padding %d bytes\n", padMessageLength);
    while (padMessageLength)
    {
        plainText.push_back(0x00);
        padMessageLength--;
    }

    unsigned char *input = &plainText[0];
    unsigned char output[plainText.size()];

    // input=reinterpret_cast<unsigned char*> (plainText.front());
    input_len = plainText.size();

    ret = mbedtls_aes_setkey_enc(&aes, &passedKey[0], 128);
    if (ret != 0)
    {
        PRINTF("mbedtls Encrypt set key failed %d\n", ret);
    }

    ret = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, plainText.size(), &iv_copy[0], input, output);
    if (ret != 0)
    {
        PRINTF("mbedtls_aes_crypt_cbc failed %d\n", ret);
    }
    printU8("Output:", output, plainText.size());
    //1
    plainText.clear();
    std::copy(output, output + input_len, back_inserter(plainText));
    //2
    //plainText=VU_8(output[0],input_len);
    return ret;
}

int CryptoLib::generateIV(AU_8_16 &iv)
{
    /* 
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;

    char *pers = "aes generate iv";
    int ret;

    mbedtls_entropy_init(&entropy);

    ret=mbedtls_entropy_add_source(&entropy,mbedtls_hardware_poll,NULL,0,MBEDTLS_ENTROPY_SOURCE_WEAK);
    if(ret)
    {
        PRINTF("mbedtls_entropy_add_source ret=%d\n");
    }
    mbedtls_ctr_drbg_init(&ctr_drbg);

    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                                     (unsigned char *)pers, strlen(pers))) != 0)
    {
        PRINTF(" failed\n ! mbedtls_ctr_drbg_init returned -0x%04x\n", -ret);
        goto exit;
    }

    if ((ret = mbedtls_ctr_drbg_random(&ctr_drbg, iv, 16)) != 0)
    {
        PRINTF(" failed\n ! mbedtls_ctr_drbg_random returned -0x%04x\n", -ret);
        goto exit;
    }
 */

    int ret = 0;
    AU_8_16 newIV;
    getRandomIV(newIV);

    printU8("GeneratedIV:", newIV);
    std::copy(newIV.begin(), newIV.end(), iv.begin());

exit:
    return ret;
}

/* 
    generate_iv_and_encrypt(vector &msg,unsigned char* iv)
    
    1- generate iv
    2- pass to encrypt_cbc
    3- return cipher and iv

    How to use :
        * create iv buffer with 16 bytes long
        * pass msg vector and iv
*/

int CryptoLib::generate_iv_and_encrypt(VU_8 &plainText, AU_8_16 &iv, const AU_8_16 &key)
{
    if (generateIV(iv) != 0)
    {
        iv[1] = iv[3] = 0xff;
    }

    int ret;

    if ((ret = encrypt_cbc(plainText, iv, key)) != 0)
    {
        PRINTF(" failed\n ! encrypt_cbc returned -0x%04x\n", -ret);
        goto exit;
    }

exit:
    return ret;
}


void CryptoLib::prependIV(VU_8 &cipherText, AU_8_16 &iv)
{
    cipherText.insert(cipherText.begin(), iv.begin(), iv.end()); // push_front, front_inserter.
}
//encrypt_and_cmac(vector &plainText)
/* 
    1- generate_iv_and_encrypt(plainText,iv)
    2- calculate cmac
    3- append with cmac
    4- prepend with iv
*/
int CryptoLib::encrypt_and_cmac(VU_8 &plainText, const AU_8_16 &key)
{
    AU_8_16 iv;
    int ret;
    ret = generate_iv_and_encrypt(plainText, iv, key);
    PRINTF("generate_iv_and_encrypt ret=%d\n", ret);
    ret = cmac_message(plainText, key);
    PRINTF("cmac_message ret=%d\n", ret);
    prependIV(plainText, iv);
    return ret;
}

//decrypt_cbc(vector &encryptedText,iv) Decrypt with predefined input size + one IV
/* 
    1- decrypts using key and iv
    2- returns the message to the source.
*/
int CryptoLib::decrypt_cbc(VU_8 &encryptedText, AU_8_16 &iv, const AU_8_16 &_key)
{

    int input_len = encryptedText.size();
    uint8_t messageDecrypt[input_len];

    PRINTF("message length=%d\n", input_len);

    int ret = 0;

    mbedtls_aes_context ctx;

    mbedtls_aes_init(&ctx);
    ret = mbedtls_aes_setkey_dec(&ctx, (uint8_t *)&_key, 128);
    if (ret != 0)
    {
        PRINTF("mbedtls decryption set key failed %d\n", ret);
        goto exit;
    }
    ret = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT, input_len, (uint8_t *)&iv[0], (const uint8_t *)&encryptedText[0], &messageDecrypt[0]);
    if (ret != 0)
    {
        PRINTF("mbedtls decryption failed %d\n", ret);
        goto exit;
    }
    mbedtls_aes_free(&ctx);
    printU8("messageDycrypt:", messageDecrypt, input_len);
    encryptedText.clear();
    std::copy(messageDecrypt, messageDecrypt + input_len, back_inserter(encryptedText));
// encryptedText = messageDecrypt;
exit:
    return ret;
}

//parse_and_decrypt(vector &ciphertext) Pass all ciphertext along with IV
int CryptoLib::parse_and_decrypt(VU_8 &ciphertext, const AU_8_16 &_key)
{
    if (ciphertext.size() < 16)
        return -1;

    AU_8_16 iv;
    std::copy_n(ciphertext.begin(), 16, iv.begin());
    ciphertext.erase(ciphertext.begin(), ciphertext.begin() + 16); // remains the rest of the ciphertext.

    return decrypt_cbc(ciphertext, iv, _key);
}
