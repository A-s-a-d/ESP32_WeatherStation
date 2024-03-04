#include "DS1621.h"

DS1621 ::DS1621()
{
    _adress = 0;
    _temperature = 0;
}

bool DS1621 ::begin(uint8_t adress)
{
    _adress = adress;
    bool res = Wire.begin();
    //

    Wire.beginTransmission(_adress);
    Wire.write(R_DS1621_config);
    Wire.write(0x03); // one shot
    Wire.endTransmission();

    return res;
}
float DS1621 ::getTemperature()
{
    uint8_t MSB = 0;
    uint8_t LSB = 0;
    float temperature = 0;

    uint8_t fraction = 0;

    Wire.beginTransmission(_adress);
    Wire.write(R_DS1621_read_temp);
    Wire.endTransmission();
    //
    Wire.requestFrom(_adress, byte(2));
    MSB = Wire.read();
    LSB = Wire.read();
    Wire.endTransmission();
    //
    fraction = LSB & 0b10000000;
    // MSB = 0b11001001;
    if (MSB & 0x80)
    {
        temperature = 0xff ^ MSB;
        temperature = -(temperature + 0x01);
        if (fraction == 0x80)
        {
            temperature = temperature - 0.5;
        }
    }
    else
    {
        temperature = MSB;
        if (fraction == 0x80)
        {
            temperature = temperature + 0.5;
        }
    }
    _temperature = temperature;
    return temperature;
}

void DS1621 ::startConv()
{
    Wire.beginTransmission(_adress);
    Wire.write(R_DS1621_start_conv);
    Wire.endTransmission();
}
void DS1621::stopConv()
{
    Wire.beginTransmission(_adress);
    Wire.write(R_DS1621_stop_conv);
    Wire.endTransmission();
}