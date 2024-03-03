#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Ticker.h>
#include <TSL2591.h>

const uint8_t A_TSL2591 = 0x29; // adresse de TSL2591

TSL2591 cap_tsl2591;

void action();

const uint32_t PERIOD = 3000; // preiode action

Ticker mticker(action, PERIOD, 0, MILLIS);

// void Byte_CONFIG()
// {
//   bool _I_R_TSL2591_CONFIG_SRESET = 0;

//   uint8_t BYTE_CONFIG = ((Byte_CONFIG + _I_R_TSL2591_CONFIG_SRESET) << 7) +
//                         (_gain << 4) +
//                         _time;
//   test_aff_txt_val_format("la configuration est : 0b", READ_CONFIG, BIN);
// }
void setup()
{
  Serial.begin(115200);
  Wire.begin();
  mticker.start();

  cap_tsl2591.begin(A_TSL2591);
  // cap_tsl2591.getId();
  cap_tsl2591.config(cap_tsl2591.GAIN_MED, cap_tsl2591.ATIME_300);

  // cap_tsl2591.calculateLux();
}

void loop()
{

  mticker.update(); /*  enlever cette ligne de commentaire pour utiliser mticker.action */
}

void action()
{
  // cap_tsl2591.getId();

  Serial.print("LUX =");
  Serial.println(cap_tsl2591.calculateLux());
}