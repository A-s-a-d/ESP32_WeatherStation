#ifndef TSL2591_H
#define TSL2591_H
#include <Arduino.h>
#include <Wire.h>

// les registres

/*
    R_ pour registre
    I_ pour une valeur dans le registre
    TOUT MAJUSCULE apres TSL2591 nom de registre
    a la fin tout minuscule nom/identification de valeur,
*/

#define R_TSL2591_ENABLE 0X00
#define R_TSL2591_CONFIG 0X01
#define R_TSL2591_AILTL 0X04
#define R_TSL2591_AILTH 0X05
#define R_TSL2591_AIHTL 0X06
#define R_TSL2591_AIHTH 0X07
#define R_TSL2591_NPAILTL 0X08
#define R_TSL2591_NPAILTH 0X09
#define R_TSL2591_NPAIHTL 0X0A
#define R_TSL2591_NPAIHTH 0X0B
#define R_TSL2591_PERSIST 0X0C
#define R_TSL2591_PID 0X11
#define R_TSL2591_ID 0X12
#define R_TSL2591_STATUS 0X13
#define R_TSL2591_C0DATAL 0X14
#define R_TSL2591_C0DATAH 0X15
#define R_TSL2591_C1DATAL 0X16
#define R_TSL2591_C1DATAH 0X17

#define I_R_TSL2591_ID_id 0X50

#define I_R_TSL2591_COMMAND_transaction_normal 32 // bit 5 et 6 de COMMAND, ou 01 mode normale , decimale 32 sur 1 byte
#define I_R_TSL2591_COMMAND_cmd 128               // cmd = 1 pour command registre, MSB donc en decimale = 128

class TSL2591
{
public:
    enum AGain
    {
        GAIN_LOW = 0x00,
        GAIN_MED = 0x01,
        GAIN_HIGH = 0x02,
        GAIN_MAX = 0x03
    };
    enum ATime
    {
        ATIME_100 = 0x00,
        ATIME_200 = 0x01,
        ATIME_300 = 0x02,
        ATIME_400 = 0x03,
        ATIME_500 = 0x04,
        ATIME_600 = 0x05,
    };
    // constructeur
    TSL2591();
    // les méthodes
    void begin(uint8_t i2cAdress);
    uint8_t getId();
    void config(uint8_t gain, uint8_t time);
    void enable();
    void disable();
    uint16_t getFullSpectrum();     /* CH0 */
    uint16_t getInfraRedSpectrum(); /* CH1 */
    uint16_t getVisibleSpectrum();
    float calculateLux();
    void set_time_gain();

    uint16_t Valeur_time;
    uint16_t Valeur_gain;

private:
    uint8_t _adress;
    uint8_t _gain;
    uint8_t _time;
    uint32_t _ALLData;

    /* _ALLData pour faire les calcules avec juste une mesure
    parceque quand on va appeler _getALLDatas();
    en getFullSpectrum, getInfraRedSpectrum et getVisibleSpectrum
    ca va faire les calcules avec des musures pris a differentes temps donc ca peux changer
    les resultats.
    */
    uint32_t _getAllDatas();
    uint8_t _Calc_R_COMMAND(uint8_t Registre);
};

#endif
