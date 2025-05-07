#include <Arduino.h>
#include <Wire.h>

#include "lol_rgb_shield.h"

#define I2C_ADDRESS 0x14
#define NEOPIXEL_PIN 17

Adafruit_NeoPixel rgbLed = Adafruit_NeoPixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
LolRgbShieldTaskHandler lolRgbShield;
char buffer[LolRgbShieldTaskHandler::MaxMessageSize];
static int bufferIndex = 0;

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

    // read chunk into buffer, discard extra bytes if past buffer size
    while (Wire.available())
    {
      uint8_t byte = Wire.read();
      if (bufferIndex < LolRgbShieldTaskHandler::MaxMessageSize - 1)
      {
        buffer[bufferIndex++] = byte;
      }
    }
    buffer[bufferIndex] = '\0';

    // last chunk
    if (bufferIndex > 0 && (buffer[bufferIndex - 1] == '\n' || bufferIndex >= LolRgbShieldTaskHandler::MaxMessageSize - 1))
    {
      if (buffer[bufferIndex - 1] == '\n')
      {
        buffer[--bufferIndex] = '\0';
      }

      lolRgbShield.setMessage(buffer);
      bufferIndex = 0;
    }
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

  Wire.begin(I2C_ADDRESS); // Initialize as I2C slave with address 0x13
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