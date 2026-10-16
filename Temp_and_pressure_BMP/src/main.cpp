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
    byte error = Wire.endTransmission();

  if (error != 0)
  {
      Serial.print("I2C error: ");
      Serial.println(error);
      return 0;
  }

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

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  Wire.beginTransmission(0x76);
  Wire.write(0xF4);
  Wire.write(0x27);
  Wire.endTransmission();
}

void loop() {
  byte tempMSB = readRegister(0xFA);
  byte tempLSB = readRegister(0xFB);
  byte tempXLSB = readRegister(0xFC);

  byte pressureMSB = readRegister(0xF7);
  byte pressureLSB = readRegister(0xF8);
  byte pressureXLSB = readRegister(0xF9);

  Serial.println("----------------");
  uint32_t rawTemp = ((uint32_t)tempMSB<<12)|((uint32_t)tempLSB<<4)|((uint32_t)tempXLSB>>4);
  uint32_t rawPressure = ((uint32_t)pressureMSB<<12)|((uint32_t)pressureLSB<<4)|((uint32_t)pressureXLSB>>4);

  uint16_t dig_T1 = read16(0x88);
  int16_t  dig_T2 = (int16_t)read16(0x8A);
  int16_t  dig_T3 = (int16_t)read16(0x8C);

  uint16_t dig_P1 = read16(0x8E);
  int16_t dig_P2 = (int16_t)read16(0x90);
  int16_t dig_P3 = (int16_t)read16(0x92);
  int16_t dig_P4 = (int16_t)read16(0x94);
  int16_t dig_P5 = (int16_t)read16(0x96);
  int16_t dig_P6 = (int16_t)read16(0x98);
  int16_t dig_P7 = (int16_t)read16(0x9A);
  int16_t dig_P8 = (int16_t)read16(0x9C);
  int16_t dig_P9 = (int16_t)read16(0x9E);

  int32_t Temp_crc1,Temp_crc2,T,t_fine;
  Temp_crc1 = ((((rawTemp >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
  Temp_crc2 = (((((rawTemp >> 4) - ((int32_t)dig_T1)) *
          ((rawTemp >> 4) - ((int32_t)dig_T1))) >> 12) *
          ((int32_t)dig_T3)) >> 14;
  t_fine = Temp_crc1 + Temp_crc2;
  T = ((t_fine)*5+128) >> 8;

  float Temperature = T / 100.0f;

  Serial.print("Temp is :");
  Serial.print(Temperature);
  Serial.println("℃");

 int64_t Press_crc1, Press_crc2, p;

  Press_crc1 = ((int64_t)t_fine) - 128000;

  Press_crc2 = Press_crc1 * Press_crc1 * (int64_t)dig_P6;
  Press_crc2 = Press_crc2 + ((Press_crc1 * (int64_t)dig_P5) << 17);
  Press_crc2 = Press_crc2 + (((int64_t)dig_P4) << 35);

  Press_crc1 = ((Press_crc1 * Press_crc1 * (int64_t)dig_P3) >> 8) +
          ((Press_crc1 * (int64_t)dig_P2) << 12);

  Press_crc1 = (((((int64_t)1) << 47) + Press_crc1) *
          (int64_t)dig_P1) >> 33;

  if (Press_crc1 == 0){
    Serial.println("Error in Pressure Correction variable");
  } else {

  p = 1048576 - rawPressure;

  p = (((p << 31) - Press_crc2) * 3125) / Press_crc1;

  Press_crc1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  Press_crc2 = (((int64_t)dig_P8) * p) >> 19;

  p = ((p + Press_crc1 + Press_crc2) >> 8) +
      (((int64_t)dig_P7) << 4);
  
  float Pressure=p/256.0f;
  
  Serial.print("Pressure is :");
  Serial.print(Pressure);
  Serial.println("Pa");

  }
  delay(2000);
}