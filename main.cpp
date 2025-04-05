#include <Arduino.h>
#include <Wire.h>

#include "lol_rgb_shield.h"

LolRgbShieldTaskHandler lolRgbShield;

Adafruit_NeoPixel rgbLed = Adafruit_NeoPixel(1, 17, NEO_GRB + NEO_KHZ800);

void receiveEvent(int bytesReceived)
{
  if (bytesReceived < 2)
    return;

  uint8_t command = Wire.read();
  if (command == 0x00)
  {
    bool state = Wire.read();
    lolRgbShield.setDisplay(state);
  }
  else if (command == 0x01)
  {
    char buffer[LolRgbShieldTaskHandler::MaxMessageSize];

    int i = 0;
    while (Wire.available() && i < sizeof(buffer) - 1)
    {
      buffer[i++] = Wire.read();
    }
    buffer[i] = '\0';

    lolRgbShield.setMessage(buffer);
  }
  else if (command == 0x02)
  {
    uint8_t scrollSpeed = Wire.read();
    lolRgbShield.setScrollSpeed(scrollSpeed);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting setup...");

  Wire.begin(0x13); // Initialize as I2C slave with address 0x13
  Wire.onReceive(receiveEvent);

  Serial.println("Setup complete");
}

void loop()
{
}

void setup1()
{
  rgbLed.begin();
  rgbLed.setBrightness(30);
  rgbLed.setPixelColor(0, 0xFF, 0x77, 0x00);
  rgbLed.show();

  lolRgbShield.setup();
}

void loop1()
{
  lolRgbShield.tick();
}