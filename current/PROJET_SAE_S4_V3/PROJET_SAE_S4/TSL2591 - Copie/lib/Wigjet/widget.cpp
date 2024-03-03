#include <Widget.h>
#define MAX 3000

Gauge::Gauge(GFXcanvas1 *pcanvas, uint16_t centerX, uint16_t centerY, uint16_t radius)
{
    _lux = 0;
}

void Gauge ::_drawBackground()
{
    _canvas->setCursor(43, 0);
    _canvas->setTextSize(1);
    _canvas->setTextColor(WHITE);
    _canvas->print("LUMIERE");

    _canvas->drawRect(9, 10, 110, 15, WHITE);

    _canvas->setRotation(1);
    _canvas->setCursor(28, 110);
    _canvas->setTextSize(1);
    _canvas->print('0');
    _canvas->setRotation(0);

    for (uint8_t i = 0, n = 0; i < 10; i++)
    {
        n += 3;
        Serial.printf("i = %d et n =%d \n", i, n * 100);
    }

    for (int i = 0; i < 10; i++)
    {
        _canvas->fillRect((MAX / 10) * i, 10, 110, 15, WHITE);
    }
}

void Gauge ::draw()
{
    _canvas->fillScreen(0);
    _drawBackground();
    _drawlux();
}

void Gauge ::_drawlux()
{
}
