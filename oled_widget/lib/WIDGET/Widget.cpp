#include <Widget.h>

AnaClock ::AnaClock(GFXcanvas1 *pcanvas, uint16_t centerX, uint16_t centerY, uint16_t radius)
{
    _radius = radius;
    _centerX = centerX;
    _centerY = centerY;
    _hour = 0;
    _minute = 0;
    _second = 0;
    _day = "0";
    _number = 0;
    _month = "0";
    _canvas = pcanvas;
}

void AnaClock ::update(uint8_t thehour, uint8_t theminute, uint8_t thesecond, String day,
                       uint8_t number, String month)
{
}

void AnaClock ::draw()
{
    _canvas->fillScreen(0);
    _drawBackground();
    _drawSecond();
    _drawMinute();
    _drawHour();
    _drawDate();
}

void AnaClock ::_drawBackground()
{
    float xx = 0;
    float yy = 0;
    uint8_t i = 0;

    for (double teta = 0.0; teta < 2.0 * PI; teta += PI / 6.0) // if i use float teta only 0*PI works in the rest numbers are off.
    {
        xx = _calculX(_radius, teta);
        yy = _calculY(_radius, teta);

        if (i >= 13)
        {
            i = 0;
        }
        if (i % 3)
        {
            _canvas->drawCircle(xx, yy, 1, WHITE);
        }
        else
        {
            if (i + 3 == 12)
            {
                _canvas->setCursor(xx - 4, yy - 4);
                _canvas->setTextSize(1);
                _canvas->setTextColor(WHITE);
                _canvas->println(i + 3);
            }
            else
            {
                _canvas->setCursor(xx - 2, yy - 4);
                _canvas->setTextSize(1);
                _canvas->setTextColor(WHITE);
                _canvas->println(i + 3);
            }
        }
        i++;
    }
    _canvas->fillCircle(_centerX, _centerY, 2, WHITE);

    _canvas->drawRect(_centerX + _radius + 6,
                      4,
                      122 - _centerX - _radius /* 128 - _centerX - _radius  - 6*/,
                      56, WHITE);

    _canvas->drawLine(_centerX + _radius + 6,
                      16,
                      128,
                      16,
                      WHITE);

    _canvas->setCursor(_centerX + _radius + 20, 6);

    _canvas->println("DS3231");
}

void AnaClock ::_drawHour()
{

    float angle_h = (TWO_PI / 12) * cap_ds3231.getHour() - PI / 2;
    // _canvas->drawLine(_centerX, _centerY, _calculX(_radius - _radius * 0.4, angle_s), _calculY(_radius - _radius * 0.4, angle_s), WHITE);
    _canvas->drawTriangle(_calculX(_radius - _radius * 0.4, angle_h), _calculY(_radius - _radius * 0.4, angle_h),
                          _calculX(2, angle_h + PI / 2), _calculY(2, -angle_h + PI / 2),
                          _calculX(2, angle_h - PI / 2), _calculY(2, angle_h - PI / 2), WHITE);
}

void AnaClock ::_drawMinute()
{
    float angle_m = (TWO_PI / 60) * cap_ds3231.getMinute() - PI / 2;
    // _canvas->drawLine(_centerX, _centerY, _calculX(_radius - _radius * 0.2, angle_s), _calculY(_radius - _radius * 0.2, angle_s), WHITE);
    _canvas->drawTriangle(_calculX(_radius - _radius * 0.1, angle_m), _calculY(_radius - _radius * 0.1, angle_m),
                          _calculX(2, angle_m + PI / 2), _calculY(2, -angle_m + PI / 2),
                          _calculX(2, angle_m - PI / 2), _calculY(2, angle_m - PI / 2), WHITE);
}

void AnaClock ::_drawSecond()
{

    float angle_s = (TWO_PI / 60) * cap_ds3231.getSecond() - PI / 2;
    _canvas->drawLine(_centerX, _centerY, _calculX(_radius, angle_s), _calculY(_radius, angle_s), WHITE);

    /*  _canvas->drawTriangle(_calculX(_radius, angle_s), _calculY(_radius, angle_s),
                           _calculX(2, angle_s + PI / 2), _calculY(2, -angle_s + PI / 2),
                           _calculX(2, angle_s - PI / 2), _calculY(2, angle_s - PI / 2), WHITE); */
}

void AnaClock ::_drawDate()
{
    String Day = cap_ds3231.getStringDay();
    String Month = cap_ds3231.getStringMonth();
    uint8_t DATE = cap_ds3231.getNumber();

    _canvas->setCursor(_centerX + _radius + 20, 20);
    _canvas->setTextSize(1);
    _canvas->setTextColor(WHITE);
    _canvas->printf("%s ", Day);
    _canvas->setCursor(_centerX + _radius + 20, 30);
    _canvas->printf("%d %s ", DATE, Month);
    _canvas->setCursor(_centerX + _radius + 20, 40);
    _canvas->printf("2024");
}

float AnaClock ::_calculX(float rayon, float angle)
{
    return _centerX + (rayon * cos(angle));
}
float AnaClock ::_calculY(float rayon, float angle)
{
    return _centerY + (rayon * sin(angle));
}

Gauge::Gauge(GFXcanvas1 *pcanvas, uint16_t centerX, uint16_t centerY, uint16_t radius)
{
    _radius = radius;
    _centerX = centerX;
    _centerY = centerY;
    _canvas = pcanvas;
    _temp = 0;
}

void Gauge ::_drawBackground()
{
    float xx = 0;
    float yy = 0;
    uint8_t i = 60;

    /* affichage 0 de gauge */
    _canvas->setCursor(0, 57);
    _canvas->setTextSize(1);
    _canvas->setTextColor(WHITE);
    _canvas->println(0);
    /* affichage 0 de gauge */

    for (double teta = 0.0; teta > -1.0 * PI; teta -= PI / 12.0) // if i use float teta only 0*PI works in the rest numbers are off.
    {
        xx = _calculX(_radius, teta);
        yy = _calculY(_radius, teta);

        if (i < 0)
        {
            i = 60;
        }

        else if (i % 10)
        {
            _canvas->drawCircle(xx, yy, 1, WHITE);
        }
        else
        {
            if (i == 60)
            {
                _canvas->setCursor(xx - 8, yy - 8);
                _canvas->setTextSize(1);
                _canvas->setTextColor(WHITE);
                _canvas->println(i);
            }
            else
            {
                _canvas->setCursor(xx - 3, yy - 4);
                _canvas->setTextSize(1);
                _canvas->setTextColor(WHITE);
                _canvas->println(i);
            }
        }
        i -= 5;
    }
    _canvas->fillCircle(_centerX, _centerY - 1, 25, WHITE);

    _canvas->setCursor(_centerX + 6, _centerY - 14);
    _canvas->setTextSize(1);
    _canvas->setTextColor(BLACK);
    _canvas->print((char)247);
    _canvas->setTextSize(2);
    _canvas->setCursor(_centerX + 12, _centerY - 14);
    _canvas->print("C");
}

void Gauge ::draw()
{
    _canvas->fillScreen(0);
    _drawBackground();
    _drawtemp();
}

void Gauge ::_drawtemp()
{
    cap_ds1621.startConv();
    float temp = cap_ds1621.getTemperature();

    float angle_s = PI + (PI / 60) * temp;
    // _canvas->drawLine(_centerX, _centerY, _calculX(_radius, angle_s), _calculY(_radius, angle_s), WHITE);

    _canvas->fillTriangle(_calculX(_radius, angle_s), _calculY(_radius, angle_s),
                          _calculX(5, angle_s + PI / 2), _calculY(5, -angle_s + PI / 2),
                          _calculX(5, angle_s - PI / 2), _calculY(5, angle_s - PI / 2), WHITE);

    _canvas->setCursor(_centerX + -22, _centerY - 8);
    _canvas->setTextSize(1);
    _canvas->setTextColor(BLACK);
    _canvas->print(temp, 2);
}
float Gauge ::_calculX(float rayon, float angle)
{
    return _centerX + (rayon * cos(angle));
}
float Gauge ::_calculY(float rayon, float angle)
{
    return _centerY + (rayon * sin(angle));
}

Gauge_LUX::Gauge_LUX(GFXcanvas1 *pcanvas)
{
    _lux = 0;
    _canvas = pcanvas;
}

void Gauge_LUX ::_drawBackground()
{
    _canvas->setCursor(43, 0);
    _canvas->setTextSize(1);
    _canvas->setTextColor(WHITE);
    _canvas->print("LUMIERE");

    _canvas->drawRect(9, 10, 110, 15, WHITE);

    _canvas->setRotation(1);
    for (int i = 0; i <= 3000; i += 300)
    {

        _canvas->setCursor(28, 116 - (11 * (i / 300)));
        // Position verticale ajustée pour chaque itération       
        _canvas->setTextSize(1);
        _canvas->print(String(i)); // Conversion de l'entier en chaîne de caractères pour l'affichage       
        if (i < 3000)
        {
            _canvas->drawLine(10, 118 - (11 * (i / 300)), 24, 118 - (11 * (i / 300)), WHITE);
        }
    }
    _canvas->setRotation(0);
}

void Gauge_LUX ::draw()
{
    _canvas->fillScreen(0);
    _drawBackground();
    _drawlux();
}

void Gauge_LUX ::_drawlux()
{
    float lux = 0;
    lux = cap_tsl2591.calculateLux();

    int longueur = (index_pixel_lux)*lux;

    _canvas->fillRect(9, 10, longueur, 15, WHITE);

    _canvas->setCursor(49, 56);
    _canvas->setTextSize(1);
    _canvas->setTextColor(WHITE);
    _canvas->print(lux);
}