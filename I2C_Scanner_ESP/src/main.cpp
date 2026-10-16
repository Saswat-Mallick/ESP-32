#include <Arduino.h>
#include <Wire.h>

void setup()
{
    Serial.begin(115200);
    Wire.begin(21, 22);

    Serial.println("Scanning I2C bus...");

    for (byte address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();

        if (error == 0)
        {
            Serial.print("Device found at 0x");
            Serial.println(address, HEX);
        }
    }

    Serial.println("Scan complete.");
}

void loop()
{
}