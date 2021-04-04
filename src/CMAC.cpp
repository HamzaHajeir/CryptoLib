#include "CMAC.h"
#include "tools.h"
#include <algorithm>


// calculate_cmac(VU_8 &msg,VU_8 &result)
/* 
    Calculate cmac
    return to result vector
*/

int CryptoLib::calculate_cmac(const VU_8 &msg,VU_8 &result, const AU_8_16 &passedKey)
{
    unsigned char output[16];
    unsigned char* input =  /* &msg[0] */ const_cast<uint8_t*>(&msg[0]);
    int input_len = msg.size();

    unsigned char key[16] = {0};
    std::copy(passedKey.begin(),passedKey.end(),key);



    int ret;
    const mbedtls_cipher_info_t *cipher_info;
    cipher_info = mbedtls_cipher_info_from_type( MBEDTLS_CIPHER_AES_128_ECB );
    if(cipher_info==NULL)
        PRINTF("mbedtls_cipher_info_from_type failed\n");

    ret = mbedtls_cipher_cmac(cipher_info, key, 128, input, input_len, output);
    PRINTF("mbedtls_cipher_cmac returned %d\n",ret);

    printU8("CMAC:", output, 16);

    std::copy(std::begin(output), std::end(output), std::back_inserter(result));
    return ret;
}

// cmac_message(vector &msg)
/* 
    1- calculate cmac
    2- append with cmac
*/
int CryptoLib::cmac_message(VU_8 &msg, const AU_8_16& key)
{

    VU_8 output;
    int ret = calculate_cmac(msg,output,key);

    std::copy(output.begin(), output.begin() + 16, back_inserter(msg));
    return ret;

}

int CryptoLib::removeMIC(VU_8 &ciphertext)
{
    ciphertext.erase(ciphertext.end()-16,ciphertext.end());
    return 0;
}

//validateMIC(vector &ciphertext) Pass all ciphertext along with IV and CMAC : returns 0 on true, failures returns false
/* 
    *   Parse ciphertext to 3 blocks
            1- IV   // removable
            2- cipher
            3- MIC_R
    **  Calculates MIC for cipher.
    *** Compares between the input(MIC_R) and calculated MIC.
*/
int CryptoLib::validateMIC(VU_8 &ciphertext, const AU_8_16 &_key, bool remove_iv)
{
    VU_8 copy;
    if(remove_iv)
    {
        std::copy(ciphertext.begin()+16,ciphertext.end(),back_inserter(copy));  //removes IV (default)
    }
    else
    {
        std::copy(ciphertext.begin(),ciphertext.end(),back_inserter(copy));
    }

    VU_8 MIC_R;
    std::copy(copy.end()-16,copy.end(),back_inserter(MIC_R));
    copy.erase(copy.end()-16,copy.end());

    VU_8 MIC_C;
    int ret = calculate_cmac(copy,MIC_C,_key);
    if(ret!=0)
    {
        PRINTF("calculate_cmac failed %d\n",ret);
    }
    bool result = std::equal(MIC_C.begin(),MIC_C.end(),MIC_R.begin());

    return ret == 0 ? !result /* returns zero at true */ : ret;
}

bool CryptoLib::isMIC(VU_8 &receivedText, const AU_8_16 &_key)
{
    return !validateMIC(receivedText, _key);
}
