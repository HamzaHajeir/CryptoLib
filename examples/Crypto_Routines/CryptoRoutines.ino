// This example demonstrate the top abstraction of CryptoLib using CBC encryption and CMAC as MIC

#include <Arduino.h>

#include <CryptoLib.h>

#include <string>
#include <vector>   //included in Crypto.h
#include <array>    //included in Crypto.h

std::string myMessage = "HelloWorld!";

void setup()
{
    Serial.begin(115200);

    AU_8_16 sharedKey;
    sharedKey.fill('5');
    VU_8 msg(myMessage.begin(), myMessage.end());

    CryptoLib::encrypt_and_cmac(msg, sharedKey);

    printU8("Encrypted data:", msg);

    // Receiving side:
    VU_8 receivedMessage = msg;

    if (CryptoLib::isMIC(receivedMessage, sharedKey))
    {
        CryptoLib::removeMIC(receivedMessage);
        CryptoLib::parse_and_decrypt(receivedMessage, sharedKey);
        printU8("Decrypted message :", receivedMessage);
        Serial.printf("Decrypted Message literal = %s\n", std::string(receivedMessage.begin(), receivedMessage.end()).c_str());
    }

}

void loop()
{
    // do nothing...
}