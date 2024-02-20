#ifndef DS1621_H
#define DS1621_H

#include <Arduino.h>
#include <Wire.h>

// les registres
#define R_DS1621_read_temp 0xAA
#define R_DS1621_start_conv 0xEE
#define R_DS1621_stop_conv 0x22
#define R_DS1621_config 0xAC

class DS1621
{
public:
    // constructeur
    DS1621();

    bool begin(uint8_t adress);
    float getTemperature();
    void startConv();
    void stopConv();

   

private:
    float _temperature;
    uint8_t _adress;
};

#endif