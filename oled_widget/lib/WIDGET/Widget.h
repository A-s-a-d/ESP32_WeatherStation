#ifndef WIDGETS_H
#define WIDGETS_H

#define mPI 3.1415926535
#define mHALF_PI 1.5707963267
#define mTWO_PI 6.2831853071

#define chardegree (char)247
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS3231.h>
#include <DS1621.h>
#include <TSL2591.h>

extern DS3231 cap_ds3231;
extern DS1621 cap_ds1621;
extern TSL2591 cap_tsl2591;

#define MAX_LUX 3000

//*********************
// anaclock widget rtc
//*********************
class AnaClock
{
public:
    // constructor
    AnaClock(GFXcanvas1 *pcanvas, uint16_t centerX, uint16_t centerY, uint16_t radius);

    void update(uint8_t thehour, uint8_t theminute, uint8_t thesecond, String day,
                uint8_t number, String month);
    void draw();

private:
    void _drawBackground();
    void _drawHour();
    void _drawMinute();
    void _drawSecond();
    void _drawDate();
    float _calculX(float rayon, float angle);
    float _calculY(float rayon, float angle);
    uint16_t _radius;
    uint16_t _centerX;
    uint16_t _centerY;
    float _hour;
    float _minute;
    float _second;
    String _day;
    uint8_t _number;
    String _month;
    GFXcanvas1 *_canvas;
};

//*********************
// gauge widget temperature
//*********************
class Gauge
{

public:
    // constructor
    Gauge(GFXcanvas1 *pcanvas, uint16_t centerX, uint16_t centerY, uint16_t radius);
    void draw();
    float _calculX(float rayon, float angle);
    float _calculY(float rayon, float angle);

private:
    void _drawBackground();
    void _drawtemp();
    uint16_t _radius;
    uint16_t _centerX;
    uint16_t _centerY;
    float _temp;
    GFXcanvas1 *_canvas;
};

//*********************
// gauge widget temperature
//*********************
class Gauge_LUX
{
public:
    // constructor
    Gauge_LUX(GFXcanvas1 *pcanvas);
    void draw();

private:
    void _drawBackground();
    void _drawlux();
    float _lux;
    GFXcanvas1 *_canvas;
};

#endif