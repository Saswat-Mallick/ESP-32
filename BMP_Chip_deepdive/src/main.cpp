#include <Arduino.h>
#include <Wire.h>

byte readRegister(byte reg)
{
  Wire.beginTransmission(0x76);
  Wire.write(reg);
  byte error = Wire.endTransmission();

  if (error != 0)
  {
      Serial.print("I2C error: ");
      Serial.println(error);
      return 0;
  }

  Wire.requestFrom(0x76, 1);

  if (Wire.available())
      return Wire.read();

  return 0;
}

uint16_t read16(byte reg)
{
    Wire.beginTransmission(0x76);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(0x76, 2);

    uint16_t value = 0;

    if (Wire.available() >= 2)
    {
        byte LSB = Wire.read();
        byte MSB = Wire.read();

        value = ((uint16_t)MSB << 8) | LSB;
    }

    return value;
}

void setup()
{
  Serial.begin(115200);
  Wire.begin(21, 22);
  Wire.beginTransmission(0x76);
  Wire.write(0xF4);
  Wire.write(0x27);
  Wire.endTransmission();
}

void loop()
{
  // byte chipID = readRegister(0xD0);

  // byte status = readRegister(0xF3);

  byte ctrl_meas = readRegister(0xF4);

  // byte config = readRegister(0xF5);

  byte tempMSB = readRegister(0xFA);
  byte tempLSB = readRegister(0xFB);
  byte tempXLSB = readRegister(0xFC);

  byte pressureMSB = readRegister(0xF7);
  byte pressureLSB = readRegister(0xF8);
  byte pressureXLSB = readRegister(0xF9);

  // Serial.print("Chip ID  = 0x");
  // Serial.println(chipID, HEX);

  // Serial.print("Status   = 0x");
  // Serial.println(status, HEX);

  Serial.print("CTRL_MEAS = 0x");
  Serial.println(ctrl_meas, HEX);

  // Serial.print("CONFIG   = 0x");
  // Serial.println(config, HEX);

  // Serial.print("FA = 0x");
  // Serial.println(tempMSB, HEX);

  // Serial.print("FB = 0x");
  // Serial.println(tempLSB, HEX);

  // Serial.print("FC = 0x");
  // Serial.println(tempXLSB, HEX);

  // Serial.print("F7 = 0x");
  // Serial.println(pressureMSB, HEX);

  // Serial.print("F8 = 0x");
  // Serial.println(pressureLSB, HEX);

  // Serial.print("F9 = 0x");
  // Serial.println(pressureXLSB, HEX);

  Serial.println("----------------");

  u_int32_t rawTemp = (tempMSB<<12)|(tempLSB<<4)|(tempXLSB>>4);
  Serial.print("The raw temperature data is ");
  Serial.println(rawTemp,BIN);

  u_int32_t rawPressure = (pressureMSB<<12)|(pressureLSB<<4)|(pressureXLSB>>4);
  Serial.print("The raw pressure data is ");
  Serial.println(rawPressure,BIN);

  uint16_t dig_T1 = read16(0x88);
  int16_t  dig_T2 = (int16_t)read16(0x8A);
  int16_t  dig_T3 = (int16_t)read16(0x8C);

  Serial.print("dig_T1 = ");
  Serial.println(dig_T1);

  Serial.print("dig_T2 = ");
  Serial.println(dig_T2);

  Serial.print("dig_T3 = ");
  Serial.println(dig_T3);

  uint16_t dig_P1 = read16(0x8E);
  int16_t dig_P2 = (int16_t)read16(0x90);
  int16_t dig_P3 = (int16_t)read16(0x92);
  int16_t dig_P4 = (int16_t)read16(0x94);
  int16_t dig_P5 = (int16_t)read16(0x96);
  int16_t dig_P6 = (int16_t)read16(0x98);
  int16_t dig_P7 = (int16_t)read16(0x9A);
  int16_t dig_P8 = (int16_t)read16(0x9C);
  int16_t dig_P9 = (int16_t)read16(0x9E);

  Serial.print("dig_P1 = ");
  Serial.println(dig_P1);

  Serial.print("dig_P2 = ");
  Serial.println(dig_P2);

  Serial.print("dig_P3 = ");
  Serial.println(dig_P3);

  Serial.print("dig_P4 = ");
  Serial.println(dig_P4);

  Serial.print("dig_P5 = ");
  Serial.println(dig_P5);

  Serial.print("dig_P6 = ");
  Serial.println(dig_P6);

  Serial.print("dig_P7 = ");
  Serial.println(dig_P7);

  Serial.print("dig_P8 = ");
  Serial.println(dig_P8);

  Serial.print("dig_P9 = ");
  Serial.println(dig_P9);

  delay(2000);
}