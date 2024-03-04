/* *******************************************  LIBRARIES  ******************************************* */
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Ticker.h>
#include <Widget.h>
#include <DS3231.h>
#include <DS1621.h>

/* ******************************************* CONSTANTS  & variables ******************************************* */

uint32_t startMillis; // some global variables available anywhere in the program
uint32_t currentMillis;
const uint16_t SCREEN_period = 2000; // the value is a number of milliseconds
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define A_OLED 0x3C
#define OLED_RESET -1
const uint8_t A_DS3231 = 0x68;  // adresse de DS3231
const uint8_t A_DS1621 = 0x49;  // adresse de DS1621
const uint8_t A_TSL2591 = 0x29; // adresse de TSL2591

#define pressbutton 26
#define ledpin 25
#define pot_analog_pin 34
bool screen_state = 0;
uint16_t pot_value = 0;
/* ************************************* FONCTION PROTOTYPES ************************************** */
void action();
void IRAM_ATTR toggleLED();
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

  //
  pinMode(ledpin, OUTPUT);
  pinMode(pressbutton, INPUT);
  attachInterrupt(pressbutton, toggleLED, FALLING);
}

/* ******************************************* LOOP ******************************************* */

void loop()
{
  mticker.update();
}

void IRAM_ATTR toggleLED()
{
  screen_state = !screen_state;
  digitalWrite(ledpin, screen_state);
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
  static uint8_t condition_screen = 0;
  pot_value = analogRead(pot_analog_pin);
  Serial.print("Analog value: ");
  Serial.println(pot_value);

  switch (condition_screen)
  {
  case 0: /*  RTC */
    anaclock.draw();
    break;
  case 1: /* TEMP */
    gauge.draw();
    break;
  case 2:
    gauge_lux.draw();
    break;
  default:
    condition_screen = 0;
    break;
  }

  if (screen_state)
  {
    currentMillis = millis(); // Get the current time
    if (currentMillis - startMillis >= SCREEN_period)
    {
      condition_screen++;
      if (condition_screen >= 4)
      {
        condition_screen = 0;
      }
      startMillis = currentMillis; // Reset the timer
    }
  }
  else
  {
    if (pot_value <= 1023)
    {
      condition_screen = 0;
    }
    if (pot_value <= 2046 & pot_value > 1023)
    {
      condition_screen = 1;
    }
    if (pot_value <= 3069 & pot_value > 2046)
    {
      condition_screen = 2;
    }
    if (pot_value < 3069)
    {
    }
  }

  oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0);
  oled.display();
}
