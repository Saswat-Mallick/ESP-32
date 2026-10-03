#include <Arduino.h>
#include <Wire.h>

void setup()
{
    Serial.begin(115200);
    Wire.begin(21, 22);
    Serial.println("BMP Chip ID test");
}

void loop()
{
    Wire.beginTransmission(0x76);
    Wire.write(0xD0);
    Wire.endTransmission();

    Wire.requestFrom(0x76,1);

    if(Wire.available()){
        byte ChipId = Wire.read();

        Serial.print("ChipId is 0x");
        Serial.println(ChipId,HEX);
    }

    delay(3000);
}