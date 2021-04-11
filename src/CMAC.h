#pragma once 

#include "CryptoLibCommon.h"
#include "mbedtls/cmac.h"

namespace CryptoLib {

    // calculate_cmac(VU_8 &msg,VU_8 &result)
    /* 
    Calculates cmac, and return to the result vecteor 
    How to use : 
        * Create result *buffer* std::vector<uint8_t>
        * Pass the data vector along the Key to calculate CMAC with.    
*/
    int calculate_cmac(const VU_8 &msg, VU_8 &result, const AU_8_16 &); // Could be returned to AU_8_16

    // cmac_message(vector &msg)
    /* 
    Appends the message with its CMAC.
    1- calculates CMAC
    2- appends with CMAC

    Returns the message appended with its CMAC code.

    How to use : 
        * Pass the data vector along the Key to append CMAC with.
*/
    int cmac_message(VU_8 &msg, const AU_8_16 &);



    //removeMIC(VU_8 &ciphertext) Removes the MIC from the ciphertext.
    /*
    Removes last 16 bytes of the ciphertext, Which are MIC (CMAC)

    Returns resuls to the same source.

    How to use :
    * Receive the message, Validate with MIC (necessary logical) before using this tool
    * Pass the ciphertext
*/
    int removeMIC(VU_8 &ciphertext);

    //validateMIC(vector &ciphertext, const AU_8_16 &, bool remove_iv = true) Pass all ciphertext along with IV and CMAC : returns 0 on success / 1 or return by calls on failure
    /* 
    Validates whether the received ciphertext has integrity at all or not.
    By default it removes the IV from the beginning of the message (pass false if intended to keep it (Or you've removed it before))

    *   Parses the ciphertext to 3 blocks
            1- IV   // removable
            2- cipher
            3- MIC_R
    **  Calculates MIC for cipher.
    *** Compares between the input(MIC_R) and calculated MIC.
    
    Returns 0 on success.
    Error code (1 or mbedtls specific error code on failure)

    How to use : 
        * Pass the ciphertext received, and the key used for the communcaiton.
*/
    int validateMIC(VU_8 &ciphertext, const AU_8_16 &, bool remove_iv = true);

    //isMIC(vector &ciphertext, const AU_8_16 &) Pass all ciphertext along with IV and CMAC : returns true/false
    /* 
    Checks whether the cipher has integrity or not.

    Returns true/false.

    How to use : 
        * Pass the receivedText and the key used for the communcation
*/
    bool isMIC(VU_8 &receivedText, const AU_8_16 &);

}