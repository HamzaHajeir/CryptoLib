// This example demonstrate a calculation of CMAC of a message

#include <Arduino.h>

#include <Crypto.h>

#include <string>
#include <vector>   //included in Crypto.h
#include <array>    //included in Crypto.h

std::string myMessage = "HelloWorld!";

void setup()
{
    Serial.begin(115200);
    VU_8 msg(myMessage.begin(),myMessage.end());    // VU_8 is std::vector<uint8_t>

    AU_8_16 key;                                    //AU_8_16 is std::array<uint8_t,16>, containing a size of 16 bytes.
    key.fill('0');

    std::string key_str(key.begin(), key.end());
    AU_8_16 iv;
    Serial.println();
    Serial.printf("Calculating CMAC to:%s with key %s\n", msg.data(), key_str.c_str());
    VU_8 CMACced;
    CryptoLib::cmac_message(msg, key);

    printU8("Result:", msg);

    

    int ret = CryptoLib::validateMIC(msg,key,false);
    bool validateMessage = ret == 0;
    Serial.printf("Received message: %s\n", std::string(msg.begin(), msg.end()).c_str());
    Serial.printf("the received message has %s\n", validateMessage ? "integrity" : "no integrity!");


    //Try tweaking the received message:
    msg.at(5) = 't';
    Serial.printf("Try tweaking the received message to %s\n", std::string(msg.begin(), msg.end()).c_str());
    ret = CryptoLib::validateMIC(msg, key, false);
    validateMessage = ret == 0;
    Serial.printf("the received message has %s\n", validateMessage ? "integrity" : "no integrity!");
}

void loop()
{
    // do nothing...
}