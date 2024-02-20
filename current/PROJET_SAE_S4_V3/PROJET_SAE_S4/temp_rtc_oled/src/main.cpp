/* *******************************************  LIBRARIES  ******************************************* */
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BusIO_Register.h>
#include <Wire.h>
#include "DS1621.h"
#include "DS3231.h"
#include <Ticker.h>

/* ******************************************* CONSTANTS ******************************************* */
const uint8_t A_DS3231 = 0x68; // adresse de DS3231
const uint8_t A_DS1621 = 0x49; // adresse de DS1621

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define A_OLED 0x3C
#define OLED_RESET -1

/* ************************************* FONCTION PROTOTYPES ************************************** */
void action();
void screen_init();
void screen_center_loading_bar();
void screen_center_pret();
void screen_ESP_TEXT();
void screen_credit();
void oled_temperature();
void oled_RTC();
/* ******************************************* OTHER ******************************************** */

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Ticker mticker(action, 1000, 0, MILLIS);
DS3231 cap_ds3231;
DS1621 cap_ds1621;

/* ******************************************* SETUP ******************************************* */

void setup()
{
  Wire.begin();

  Serial.begin(115200);
  // Serial.println("pret");
  // //

  // mticker.start();
  // bool res_ds3231 = cap_ds3231.begin(A_DS3231);
  // bool res_1621 = cap_ds1621.begin(A_DS1621);
  // if (res_1621 * res_ds3231 == false)
  // {
  //   Serial.println("ERROR");
  // }
  // else
  // {
  //   Serial.println("pret");
  //   mticker.start();
  // }

  if (!oled.begin(SSD1306_SWITCHCAPVCC, A_OLED))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  screen_init();
  oled_temperature(); // pour tester l'affichage, il faut pas mettre en setup
  delay(2000);
  oled_RTC(); // pour tester l'affichage, il faut pas mettre en setup
}

/* ******************************************* LOOP ******************************************* */

void loop()
{
  mticker.update();
}

/* *********************************** FONCTION DEFINITIONS *********************************** */

void action()
{
  uint8_t second = cap_ds3231.getSecond();
  uint8_t heure = cap_ds3231.getHour();
  uint8_t minute = cap_ds3231.getMinute();
  String Day = cap_ds3231.getStringDay();
  String Month = cap_ds3231.getStringMonth();
  uint8_t DATE = cap_ds3231.getNumber();
  Serial.printf(" HH : MM : SS : JOUR : MOIS : DATE = %d : %d : %d : %s : %s : %d\n \r", heure, minute, second, Day, Month, DATE);
  cap_ds1621.startConv();
  float temp = cap_ds1621.getTemperature();
  Serial.printf("temp = %d", temp);
}

void screen_init()
{
  screen_center_loading_bar(); // loading
  screen_center_pret();
  delay(2000);
  screen_ESP_TEXT();
  delay(2000);
  screen_credit();
  delay(2000);
}
void screen_center_loading_bar()
{
  oled.clearDisplay(); // Clear the display buffer
  uint8_t add = 10;    // manually changing the y position of loading bar.
  /*
   * the for loop belaw gives the diffrent lines on y axis that can be manyally be moved up or down
   * eg :
   * j is the 32 line of y axis
   * x is 33 line of y axis
   * ..... etc.
   */
  for (int y_j = 32 + add, y_k = 33 + add, y_f = 34 + add, y_g = 35 + add, y_h = 36 + add; y_j < 33 + add && y_k < 34 + add && y_f < 35 + add && y_g < 36 + add && y_h < 37 + add; y_j++)
  {
    for (int i = 0; i < 128; i++)
    {
      oled.drawPixel(i, y_j, SSD1306_WHITE);
      oled.drawPixel(i, y_k, SSD1306_WHITE);
      oled.drawPixel(i, y_f, SSD1306_WHITE);
      oled.drawPixel(i, y_g, SSD1306_WHITE);
      oled.drawPixel(i, y_h, SSD1306_WHITE);
      oled.display(); // Update the display after drawing each pixel

      oled.setTextSize(2); // Draw 2X-scale text
      oled.setTextColor(SSD1306_WHITE);
      oled.setCursor(25, 10 + add);
      oled.println("Loading");
      oled.display(); // Show initial text
    }
  }
}
void screen_center_pret()
{
  oled.setTextSize(2); // Draw 2X-scale text
  oled.clearDisplay(); // Clear the display buffer
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(40, 25);
  oled.println("Pret");
  oled.display(); // Show initial text
}

void screen_ESP_TEXT()
{
  oled.clearDisplay(); // Clear the display buffer
  oled.setTextSize(2); // Draw 2X-scale text
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 10);
  oled.println("   ESP32");
  oled.println("  Station");
  oled.println("   Meteo");

  oled.display(); // Show initial text
}

void screen_credit()
{
  oled.clearDisplay(); // Clear the display buffer
  oled.setTextSize(2); // Draw 2X-scale text
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 16);
  oled.println(".M Asad \n.M Thomas ");
  oled.display(); // Show initial text
}

// 'thermometer', 40x40px
const unsigned char img_thermometer[] PROGMEM = {
    0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x06, 0x0c, 0x00, 0x00, 0x00,
    0x0c, 0x06, 0x7e, 0x00, 0x00, 0x0c, 0x06, 0x7e, 0x00, 0x00, 0x0c, 0x46, 0x00, 0x00, 0x00, 0x0c,
    0x66, 0x00, 0x00, 0x00, 0x0c, 0x66, 0x00, 0x00, 0x00, 0x0c, 0x66, 0x78, 0x00, 0x00, 0x0c, 0x66,
    0x00, 0x00, 0x00, 0x0c, 0x66, 0x00, 0x00, 0x00, 0x0c, 0x66, 0x00, 0x00, 0x00, 0x0c, 0x66, 0x7c,
    0x00, 0x00, 0x0c, 0x66, 0x7e, 0x00, 0x00, 0x0c, 0x66, 0x00, 0x00, 0x00, 0x0c, 0x66, 0x00, 0x00,
    0x00, 0x0c, 0x66, 0x00, 0x00, 0x00, 0x0c, 0x66, 0x78, 0x00, 0x00, 0x0c, 0x66, 0x78, 0x00, 0x00,
    0x0c, 0x66, 0x00, 0x00, 0x00, 0x0c, 0x66, 0x00, 0x00, 0x00, 0x0c, 0x66, 0x00, 0x00, 0x00, 0x0c,
    0x66, 0x7e, 0x00, 0x00, 0x0c, 0x66, 0x3c, 0x00, 0x00, 0x0c, 0x66, 0x00, 0x00, 0x00, 0x1c, 0x63,
    0x00, 0x00, 0x00, 0x38, 0x61, 0x80, 0x00, 0x00, 0x30, 0xe1, 0x80, 0x00, 0x00, 0x61, 0xf0, 0xc0,
    0x00, 0x00, 0x63, 0xf8, 0xc0, 0x00, 0x00, 0x63, 0xfc, 0x40, 0x00, 0x00, 0x63, 0xfc, 0x40, 0x00,
    0x00, 0x63, 0xf8, 0xc0, 0x00, 0x00, 0x63, 0xf8, 0xc0, 0x00, 0x00, 0x21, 0xf0, 0xc0, 0x00, 0x00,
    0x30, 0x01, 0x80, 0x00, 0x00, 0x18, 0x03, 0x80, 0x00, 0x00, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x07,
    0xfe, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 224)
const int img_allArray_LEN_thermometer = 1;
const unsigned char *img_allArray_thermometer[1] = {
    img_thermometer};

void oled_temperature()
{
  oled.clearDisplay(); // Clear the display buffer
  oled.setTextColor(SSD1306_WHITE);

  oled.drawBitmap(0, 22, img_thermometer, 40, 40, SSD1306_WHITE);

  oled.setTextSize(1);       // Set text size back to 1X
  oled.setCursor(12, 13);    // Adjust the position as needed
  oled.print("Temperature"); // Include only the degree symbol

  oled.setCursor(35, 23); // Adjust the position as needed
  oled.print("DS1621");   // Include only the degree symbol

  oled.setTextSize(2); // Draw 2X-scale text
  oled.setCursor(38, 40);
  oled.println("25.25"); // ici remplacer 25.25 a remplacer par le temperature.

  oled.setTextSize(2);    // Set text size back to 1X
  oled.setCursor(98, 40); // Adjust the position as needed
  oled.print((char)247);

  oled.setTextSize(2);     // Set text size back to 1X
  oled.setCursor(110, 40); // Adjust the position as needed
  oled.print("C");

  oled.display();
}

// 'hour-glass', 40x40px
const unsigned char img_hour_glass[] PROGMEM = {
    0x01, 0xff, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xff, 0x80, 0x00,
    0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f,
    0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff,
    0xfc, 0x00, 0x00, 0x1d, 0xfe, 0x18, 0x00, 0x00, 0x18, 0x00, 0x18, 0x00, 0x00, 0x0c, 0x00, 0x30,
    0x00, 0x00, 0x06, 0x00, 0x60, 0x00, 0x00, 0x03, 0x00, 0xc0, 0x00, 0x00, 0x01, 0x81, 0x80, 0x00,
    0x00, 0x00, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
    0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00,
    0x7e, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x03, 0xff,
    0xc0, 0x00, 0x00, 0x07, 0xc3, 0xe0, 0x00, 0x00, 0x0f, 0x81, 0xf0, 0x00, 0x00, 0x1f, 0x00, 0xf8,
    0x00, 0x00, 0x1e, 0x00, 0x78, 0x00, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x00, 0x38, 0x00, 0x1c, 0x00,
    0x00, 0x70, 0x00, 0x1e, 0x00, 0x00, 0x60, 0x00, 0x0e, 0x00, 0x00, 0x60, 0x00, 0x06, 0x00, 0x00,
    0x40, 0x00, 0x02, 0x00, 0x00, 0x40, 0x00, 0x02, 0x00, 0x01, 0xff, 0xff, 0xff, 0x80, 0x03, 0xff,
    0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xff, 0x80};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 224)
const int img_allArray_LEN_hour_glass = 1;
const unsigned char *img_allArray_hour_glass[1] = {
    img_hour_glass};

void oled_RTC()
{
  oled.clearDisplay(); // Clear the display buffer
  oled.setTextColor(SSD1306_WHITE);

  oled.drawBitmap(0, 22, img_hour_glass, 40, 40, SSD1306_WHITE);

  oled.setTextSize(1);   // Set text size back to 1X
  oled.setCursor(6, 13); // Adjust the position as needed
  oled.print("TEMPS");   // Include only the degree symbol

  oled.setCursor(35, 23); // Adjust the position as needed
  oled.print("DS3231");   // Include only the degree symbol

  oled.setTextSize(2); // Draw 2X-scale text
  oled.setCursor(30, 35);
  oled.println("00:00:00"); // ici remplacer 25.25 a remplacer par le temperature.

  oled.display();
}
