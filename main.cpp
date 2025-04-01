#include <Arduino.h>
#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting setup...");

  Wire.begin();
  Serial.println("I2C initialized");
}

void loop()
{
  Wire.beginTransmission(0x08);
  Wire.write("Hello");
  Wire.endTransmission();
  delay(1000);
}