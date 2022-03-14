#include <Wire.h>

#define S11059_Addr 0x2A
#define RegisterCall 0x00
#define RegisterWrite1 0x89
#define RegisterWrite2 0x09
#define SensorRegister 0x03

int high_byte, low_byte, red, green, blue, IR;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(S11059_Addr);
  Wire.write(RegisterCall);
  Wire.write(RegisterWrite1);
  Wire.endTransmission();

  Wire.beginTransmission(S11059_Addr);
  Wire.write(RegisterCall);
  Wire.write(RegisterWrite2);
  Wire.endTransmission();
}

void loop() {
  

  delay(1000);

  Wire.beginTransmission(S11059_Addr);
  Wire.write(SensorRegister);
  Wire.endTransmission();

  Wire.requestFrom(S11059_Addr,8);
  if (Wire.available()){
    high_byte = Wire.read();
    low_byte = Wire.read();
    red = high_byte << 8 | low_byte;

    high_byte = Wire.read();
    low_byte = Wire.read();
    green = high_byte << 8 | low_byte;

    high_byte = Wire.read();
    low_byte = Wire.read();
    blue = high_byte << 8 | low_byte;

    high_byte = Wire.read();
    low_byte = Wire.read();
    IR = high_byte << 8 | low_byte;
  }
  Wire.endTransmission();

  Serial.print(red);
  Serial.print(",");
  Serial.print(green);
  Serial.print(",");
  Serial.print(blue);
  Serial.print(",");
  Serial.print(IR);
  Serial.println("");
}