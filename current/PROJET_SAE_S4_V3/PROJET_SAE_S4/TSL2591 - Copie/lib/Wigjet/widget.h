#ifndef WIDGETS_H
#define WIDGETS_H

#define chardegree (char)247
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TSL2591.h>

extern TSL2591 cap_tsl2591;

//*********************
// gauge widget
//*********************

class Gauge
{

public:
    // constructor
    Gauge(GFXcanvas1 *pcanvas, uint16_t centerX, uint16_t centerY, uint16_t radius);
    void draw();

private:
    void _drawBackground();
    void _drawlux();
    float _lux;
    GFXcanvas1 *_canvas;
};
#endif