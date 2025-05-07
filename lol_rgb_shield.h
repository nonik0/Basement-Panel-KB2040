#pragma once

#include <Adafruit_NeoMatrix.h>
#include <Fonts/TomThumb.h>

#define MATRIX_HEIGHT 5
#define MATRIX_WIDTH 14
#define MATRIX_PIN 4

// only works as static if I specify more than 4 args?
Adafruit_NeoMatrix _matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_PIN);//, NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT);

class LolRgbShieldTaskHandler
{
public:
    static const int MaxMessageSize = 500;

    LolRgbShieldTaskHandler() {}

    bool setup();
    void setDisplay(bool display) { _display = display; }
    void setMessage(const char *message);
    void setScrollSpeed(uint8_t speed);
    void tick();

private:
    static const uint8_t PIN = MATRIX_PIN;
    static const uint8_t WIDTH = MATRIX_WIDTH;
    static const uint8_t HEIGHT = MATRIX_HEIGHT;
    static const int MIN_UPDATE_INTERVAL = 10;
    static const int MAX_UPDATE_INTERVAL = 500;

    const uint16_t _colors[3] = {_matrix.Color(0xFF, 0x33, 0x00), _matrix.Color(0xFF, 0x77, 0x00), _matrix.Color(0xFF, 0x99, 0x00)};

    char _message[MaxMessageSize];
    uint16_t _message_width;
    int _x = _matrix.width();
    int _y = _matrix.height();
    int _pass = 0;
    bool _display = true;
    unsigned long _lastUpdate = 0;
    unsigned long _updateInterval = 50;
};

bool LolRgbShieldTaskHandler::setup()
{
    Serial.println("Starting LolRgbShieldTask setup");

    strcpy(_message, "STELLA IS BELLA!");

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

void LolRgbShieldTaskHandler::setMessage(const char *message)
{
    Serial.println("LolRgbShieldTask setMessage: " + String(message));

    strncpy(_message, message, MaxMessageSize - 1);
    _message[MaxMessageSize - 1] = '\0';

    int16_t d1;
    uint16_t d2;
    _matrix.getTextBounds(_message, 0, 0, &d1, &d1, &_message_width, &d2);

    _x = _matrix.width();
}

void LolRgbShieldTaskHandler::setScrollSpeed(uint8_t speed)
{
    _updateInterval = map(constrain(speed, 0, 100), 100, 0, MIN_UPDATE_INTERVAL, MAX_UPDATE_INTERVAL);
}

void LolRgbShieldTaskHandler::tick()
{
    if (millis() - _lastUpdate < _updateInterval)
    {
        return;
    }

    _lastUpdate = millis();

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
