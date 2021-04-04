// This example demonstrate a full cycle of encryption/decryption of a message using CBC 128.

#include <Arduino.h>

#include <Crypto.h>

#include <string>
#include <vector>   //included in Crypto.h
#include <array>    //included in Crypto.h

std::string myMessage = "HelloWorld!";
using namespace CryptoLib;

void setup()
{
    Serial.begin(115200);
    VU_8 msg(myMessage.begin(),myMessage.end());    // VU_8 is std::vector<uint8_t>

    AU_8_16 key;                                    //AU_8_16 is std::array<uint8_t,16>, containing a size of 16 bytes.
    key.fill('0');

    std::string key_str(key.begin(), key.end());
    AU_8_16 iv;

    generate_iv_and_encrypt(msg, iv, key);

    Serial.printf("Encrypting %s with key %s", myMessage.c_str(), key_str.c_str());
    printU8("using IV:", iv);       // Prints IV in bytes (hex)

    printU8("Encrypted MSG:", msg); // Print encrypted message in bytes (hex)

    Serial.printf("Decrypting ...\n");
    decrypt_cbc(msg, iv, key);
    printU8("Result:", msg);

    std::string decryptedMessage(msg.begin(),msg.end());
    Serial.printf("Decrypted message = %s\n",decryptedMessage.c_str());

}

void loop()
{
    // do nothing...
}