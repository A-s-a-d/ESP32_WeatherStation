#ifndef DS3231_H
#define DS3231_H

#include <Arduino.h>
#include <Wire.h>

// les registres
#define R_DS3231_second 0x00
#define R_DS3231_minute 0x01
#define R_DS3231_heures 0x02
#define R_DS3231_day 0x03
#define R_DS3231_date 0x04
#define R_DS3231_mois 0x05
#define R_DS3231_year 0x06
#define R_DS3231_controle 0x0E
#define R_DS3231_controle_status 0x0F
#define R_DS3231_aging_offset 0x10

class DS3231
{
public:
    // constructeur
    DS3231();

    // les methodes

    bool begin(uint8_t adress);
    void setTime(uint8_t hh, uint8_t mm, uint8_t ss);
    void setDate(uint8_t index_day, uint8_t num, uint8_t index_month);
    uint8_t getHour();   // hh
    uint8_t getMinute(); // mm
    uint8_t getSecond();
    String getStringDay();   // lun, Mar, Mer, Jeu, Ven, Sam, Dim
    String getStringMonth(); // Jan, Fev, Mars, Avril, Mai, Juin, Juil, Aout, Sept,// Oct, Nov, Dec
    uint8_t getNumber();     // 1 to 31
    String getStringTime();

private:
    uint8_t _adress;
    uint8_t _binToBcd(uint8_t val);
    uint8_t _bcdToBin(uint8_t val);
};

#endif