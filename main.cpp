#include <Arduino.h>

#include "lol_rgb_shield.h"
#include "tunnel_runner.h"

LolRgbShieldTaskHandler lolRgbShield;
TunnelRunnerTaskHandler tunnelRunner;

Adafruit_NeoPixel rgbLed = Adafruit_NeoPixel(1, 17, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting setup...");

  while (!tunnelRunner.setup())
  {
    Serial.println("Failed to setup TunnelRunnerTaskHandler");
    delay(5000);
  }

  Serial.println("Setup complete");
}

void loop()
{
  tunnelRunner.tick();
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