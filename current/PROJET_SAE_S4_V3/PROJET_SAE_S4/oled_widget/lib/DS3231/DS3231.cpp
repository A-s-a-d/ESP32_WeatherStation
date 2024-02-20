#include "DS3231.h"

const char *months[] = {"JAN", "FEV", "MAR", "AVR", "MAI", "JUIN", "JUIL", "AOU", "SEP", "OCT", "NOV", "DEC"};
const char *days[] = {"DIM", "LUN", "MAR", "MER", "JEU", "VEN", "SAM"};

DS3231 ::DS3231()
{
    _adress = 0;
}

bool DS3231 ::begin(uint8_t i2cAdress)
{
    _adress = i2cAdress;
    bool res = Wire.begin();
    return res;
}

uint8_t DS3231 ::_binToBcd(uint8_t val)
{
    uint8_t a = val / 16;
    a = a * 10;
    uint8_t b = val % 16; // modulo
    uint8_t s = a + b;
    return s;
}
// testé
uint8_t DS3231::_bcdToBin(uint8_t val)
{
    uint8_t a = val / 10;
    a = a * 16;
    uint8_t b = val % 10; // modulo
    uint8_t s = a + b;
    return s;
}

void DS3231 ::setTime(uint8_t hh, uint8_t mm, uint8_t ss)
{
    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_heures);
    Wire.write(_bcdToBin(hh));
    Wire.endTransmission();
    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_minute);
    Wire.write(_bcdToBin(mm));
    Wire.endTransmission();
    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_second);
    Wire.write(_bcdToBin(ss));
    Wire.endTransmission();
}

void DS3231 ::setDate(uint8_t index_day, uint8_t num, uint8_t index_month)
{
    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_day);
    Wire.write(_binToBcd(index_day));
    Wire.endTransmission();
    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_date);
    Wire.write(_binToBcd(num));
    Wire.endTransmission();
    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_mois);
    Wire.write(_binToBcd(index_month));
    Wire.endTransmission();
}
// testé
uint8_t DS3231 ::getHour() // hh
{
    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_heures);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    uint8_t heure = Wire.read();
    Wire.endTransmission();
    heure = _binToBcd(heure);
    return heure;
}
// testé
uint8_t DS3231 ::getMinute()
{
    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_minute);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    uint8_t minute = Wire.read();
    Wire.endTransmission();
    minute = _binToBcd(minute);
    return minute;
}
// testé
uint8_t DS3231 ::getSecond()
{
    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_second);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    uint8_t second = Wire.read();
    Wire.endTransmission();
    second = _binToBcd(second);
    return second;
}

String DS3231 ::getStringDay() // lun, Mar, Mer, Jeu, Ven, Sam, Dim
{
    uint8_t theindex_day = 0;

    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_day);
    Wire.endTransmission();
    //
    Wire.requestFrom(_adress, byte(1));
    theindex_day = _bcdToBin(Wire.read());
    Wire.endTransmission();

    return days[theindex_day - 1];
}

String DS3231 ::getStringMonth() // Jan, Fev, Mars, Avril, Mai, Juin, Juil, Aout, Sept,// Oct, Nov, Dec
{
    uint8_t theindex_month = 0;

    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_mois);
    Wire.endTransmission();
    //
    Wire.requestFrom(_adress, byte(1));
    theindex_month = _bcdToBin(Wire.read());
    Wire.endTransmission();

    return months[theindex_month - 1];
}

uint8_t DS3231 ::getNumber() // 1 to 31
{
    Wire.beginTransmission(_adress);
    Wire.write(R_DS3231_date);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    uint8_t date = Wire.read();
    Wire.endTransmission();

    return _bcdToBin(date);
}
