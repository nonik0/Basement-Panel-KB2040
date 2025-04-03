#pragma once

#include <Adafruit_NeoMatrix.h>
#include <Fonts/TomThumb.h>

class LolRgbShieldTaskHandler
{
private:
    static const uint8_t PIN = 4;
    static const uint8_t WIDTH = 14;
    static const uint8_t HEIGHT = 5;
    static const int DELAY_MS = 100;

    Adafruit_NeoMatrix _matrix = Adafruit_NeoMatrix(WIDTH, HEIGHT, PIN);
    const uint16_t _colors[3] = {_matrix.Color(0xFF, 0x33, 0x00), _matrix.Color(0xFF, 0x77, 0x00), _matrix.Color(0xFF, 0x99, 0x00)};
    static const int MaxMessageSize = 100;

    char _message[MaxMessageSize];
    uint16_t _message_width;   // Computed in setup() below
    int _x = _matrix.width();  // Start with message off right edge
    int _y = _matrix.height(); // With custom fonts, y is the baseline, not top
    int _pass = 0;             // Counts through the colors[] array
    bool _display = true;
    unsigned long _lastUpdate = 0;

public:
    LolRgbShieldTaskHandler() {}

    bool setup();
    void tick();
};

bool LolRgbShieldTaskHandler::setup()
{
    Serial.println("Starting LolRgbShieldTask setup");

    strcpy(_message, "BEAU IN TOW!");

    _matrix.begin();
    _matrix.setBrightness(5);
    _matrix.setFont(&TomThumb);
    _matrix.setTextWrap(false);
    _matrix.setTextColor(_colors[0]);

    int16_t d1;
    uint16_t d2;
    _matrix.getTextBounds(_message, 0, 0, &d1, &d1, &_message_width, &d2);

    Serial.println("LolRgbShieldTask setup complete");
    return true;
}

void LolRgbShieldTaskHandler::tick()
{
    if (millis() - _lastUpdate < DELAY_MS)
    {
        return;
    }

    _lastUpdate = millis();
    Serial.println("LolRgbShieldTask tick");

    if (!_display)
    {
        _matrix.fillScreen(0);
        _matrix.show();
        delay(100);
        return;
    }

    _matrix.fillScreen(0);
    _matrix.setCursor(_x, _y);
    _matrix.print(_message);
    _matrix.show();

    if (--_x < -_message_width)
    {
        _x = _matrix.width();
        if (++_pass >= 3)
        {
            _pass = 0;
        }
        _matrix.setTextColor(_colors[_pass]);
    }
}
