/* *******************************************  LIBRARIES  ******************************************* */
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Ticker.h>
#include <Widget.h>
#include <DS3231.h>
#include <DS1621.h>

/* ******************************************* CONSTANTS ******************************************* */

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define A_OLED 0x3C
#define OLED_RESET -1
const uint8_t A_DS3231 = 0x68;  // adresse de DS3231
const uint8_t A_DS1621 = 0x49;  // adresse de DS1621
const uint8_t A_TSL2591 = 0x29; // adresse de TSL2591

/* ************************************* FONCTION PROTOTYPES ************************************** */
void action();

/* ******************************************* OTHER ******************************************** */
DS3231 cap_ds3231;
DS1621 cap_ds1621;
TSL2591 cap_tsl2591;

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const uint32_t PERIOD = 1000;
// obj
Ticker mticker(action, PERIOD, 0, MILLIS);
GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
AnaClock anaclock(&canvas, 32, 32, 28);
Gauge gauge(&canvas, 64, 64, 60);
Gauge_LUX gauge_lux(&canvas);

/* ******************************************* SETUP ******************************************* */

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  if (!oled.begin(SSD1306_SWITCHCAPVCC, A_OLED))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  else
  {
    mticker.start();
  }
  bool res = cap_ds3231.begin(A_DS3231);
  cap_ds1621.begin(A_DS1621);
  // cap_ds3231.setDate(1, 3, 3);
  // cap_ds3231.setTime(17, 20, 59);

  cap_tsl2591.begin(A_TSL2591);
  // cap_tsl2591.getId();
  cap_tsl2591.config(cap_tsl2591.GAIN_MED, cap_tsl2591.ATIME_300);
}

/* ******************************************* LOOP ******************************************* */

void loop()
{
  mticker.update();
}

/* *********************************** FONCTION DEFINITIONS *********************************** */

void action()
{
  // uint8_t second = cap_ds3231.getSecond();
  // uint8_t heure = cap_ds3231.getHour();
  // uint8_t minute = cap_ds3231.getMinute();
  // String Day = cap_ds3231.getStringDay();
  // String Month = cap_ds3231.getStringMonth();
  // uint8_t DATE = cap_ds3231.getNumber();
  // Serial.printf(" HH : MM : SS : JOUR : MOIS : DATE = %d : %d : %d : %s : %s : %d  \n \r", heure, minute, second, Day, Month, DATE);

  // Serial.print("LUX =");
  // Serial.println(cap_tsl2591.calculateLux());

  // uint8_t condition_Screen = 1;
  // switch (condition_Screen)
  // {
  // case 0:
  //   anaclock.draw();
  //   condition_Screen = 1;
  //   break;
  // case 1: /*  RTC */
  //   condition_Screen = 2;
  // anaclock.draw();
  //   break;
  // case 2: /* TEMP */
  //   condition_Screen = 1;
  // gauge.draw();
  //   break;
  // default:
  //   condition_Screen = 0;
  //   break;
  // }

    // gauge.draw();
  // anaclock.draw();
  gauge_lux.draw();
  oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0);
  oled.display();
}
