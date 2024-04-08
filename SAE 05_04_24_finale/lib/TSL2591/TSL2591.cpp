#include "TSL2591.h"

TSL2591 ::TSL2591()
{
    _adress = 0;
    _gain = 0;
    _time = 0;
    _ALLData = 0;
}

void TSL2591 ::begin(uint8_t i2cAdress)
{
    _adress = i2cAdress;
}

/* Fonction testé en Main, fonnctionne. */
uint8_t TSL2591 ::_Calc_R_COMMAND(uint8_t Registre)
{
    uint8_t COMMAND_byte = (I_R_TSL2591_COMMAND_cmd + I_R_TSL2591_COMMAND_transaction_normal) + Registre;
    /* you have to send send a the adresseof register by
    writing in the command  register */
    /* command register : 1 BYTE, 1bit CMD, 2bits transaction, 5 bits @ register
    CMD 1 when adressing a register,
    Transaction for different modes, 01 for normal modes
    adress:@ of register */
    return COMMAND_byte;
}

uint8_t TSL2591 ::getId()
{
    uint8_t Byte_COMMAND = _Calc_R_COMMAND(R_TSL2591_ID);
    Wire.beginTransmission(_adress);
    Wire.write(Byte_COMMAND);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    uint8_t ID = Wire.read();
    Wire.endTransmission();

    return ID;
}

void TSL2591 ::config(uint8_t gain, uint8_t time)
{
    /* you have to send a the adresse of register by
    writing in the command  register */
    /* command register : 1 BYTE, 1bit CMD, 2bits transaction, 5 bits @ register
    CMD 1 when adressing a register,
    Transaction for different modes, 01 for normal modes
    adress:@ of register */
    uint8_t Byte_COMMAND = _Calc_R_COMMAND(R_TSL2591_CONFIG);
    _gain = gain;
    _time = time;
    uint8_t Byte_CONFIG = 0;
    /* Byte_CONFIG testé en Main, fonnctionne. */
    Byte_CONFIG = ((Byte_CONFIG + _gain) << 4) + _time;
    Wire.beginTransmission(_adress);
    Wire.write(Byte_COMMAND);
    Wire.write(Byte_CONFIG);
    Wire.endTransmission();

    Wire.beginTransmission(_adress);
    Wire.write(Byte_COMMAND);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    uint8_t READ_CONFIG = Wire.read();
    Wire.endTransmission();
    set_time_gain();
}

/* Enable Register 0X00
ALS = AMBIANT LIGHT SENSOR.
MBS 7 : NPIEN = No Persist Interrupt Enable. When asserted NP Threshold
        conditions will generate an interrupt, bypassing the persist filter
        *******not using NPI so 0.

6 SAI = Sleep after interrupt. When asserted, the device will power down at
        the end of an ALS cycle if an interrupt has been generated.
        ********not using interupt so 0.

5 RESERVED = Reserved. Write as 0.
        ********* 0.

4 AIEN = ALS Interrupt Enable. When asserted permits ALS interrupts to be
        generated, subject to the persist filter.
        ********** not using interupt so 0 but we are not sure if the sensor will start with 0  so we put 1.
3-2 RESERVED = Reserved. Write as 0.

1 AEN = ALS Enable. This field activates ALS function. Writing a one
        activates the ALS. Writing a zero disables the ALS.
        ********** 1 to active ALS fonction(enable) and 0 to disactivate ALS fonction cycle(disable).

LSB 0 PON = Power ON. This field activates the internal oscillator to permit the
            timers and ADC channels to operate. Writing a one activates the
            oscillator. Writing a zero disables the oscillator.
            ********** 1 to active oscillator to operate ADC and timer. set to 1 when starting ALS cycle
            and 0 to disactivate oscillator, set to 0 after the end of ALS cycle

*/

void TSL2591 ::enable()
{
    uint8_t Byte_COMMAND = _Calc_R_COMMAND(R_TSL2591_ENABLE);

    /* Byte_CONFIG testé en Main, fonnctionne. */
    uint8_t Byte_Enable = 0;
    Byte_Enable = 0b00010011; // PON et AEN et AIEN activer

    Wire.beginTransmission(_adress);
    Wire.write(Byte_COMMAND);
    Wire.write(Byte_Enable);
    Wire.endTransmission();
}

void TSL2591 ::disable()
{
    uint8_t Byte_COMMAND = _Calc_R_COMMAND(R_TSL2591_ENABLE);

    /* Byte_CONFIG testé en Main, fonnctionne. */

    uint8_t Byte_Enable = 0;
    Byte_Enable = 0b00000000; // PON et AEN et AIEN desactiver

    Wire.beginTransmission(_adress);
    Wire.write(Byte_COMMAND);
    Wire.write(Byte_Enable);
    Wire.endTransmission();
}

uint16_t TSL2591 ::getFullSpectrum() /* CH0 */
{
    return ((_ALLData & 0XFFFF0000) >> 16);
}

uint16_t TSL2591 ::getInfraRedSpectrum() /* CH1 */
{
    return _ALLData & 0X0000FFFF;
}

uint16_t TSL2591 ::getVisibleSpectrum()
{
    uint16_t VisibleSpectrum = (getFullSpectrum() - getInfraRedSpectrum());
    return VisibleSpectrum;
}

float TSL2591 ::calculateLux() // change uint32_t to float later
{

    float lux = 0;
    uint32_t DATA = _getAllDatas();

    if (DATA == 0xFFFFFFFF)
    {
        lux = -1;
    }
    else
    {
        float cpl = 0;
        cpl = (Valeur_time * Valeur_gain) / 408.0;

        lux = ((getVisibleSpectrum()) * (1 - getInfraRedSpectrum() / getFullSpectrum())) / cpl;
    }

    return lux;
}

void TSL2591 ::set_time_gain()
{

    switch (_time)
    {
    case 0:
        Valeur_time = 100;
        break;
    case 1:
        Valeur_time = 200;
        break;
    case 2:
        Valeur_time = 300;
        break;
    case 3:
        Valeur_time = 400;
        break;
    case 4:
        Valeur_time = 500;
        break;
    case 5:
        Valeur_time = 600;
        break;
    default:
        Valeur_time = 100;
        break;
    }

    switch (_gain)
    {
    case 0:
        Valeur_gain = 1;
        break;
    case 1:
        Valeur_gain = 25;
        break;
    case 2:
        Valeur_gain = 428;
        break;
    case 3:
        Valeur_gain = 9876;
        break;
    default:
        Valeur_gain = 1;
        break;
    }
}

uint32_t TSL2591 ::_getAllDatas()
{

    uint8_t Registre = R_TSL2591_C0DATAL;
    uint8_t Byte_COMMAND = 0;
    uint8_t C0_LSB = 0;
    uint8_t C0_MSB = 0;
    uint8_t C1_LSB = 0;
    uint8_t C1_MSB = 0;
    uint32_t DATA = 0;

    enable(); // On place la fonction enable ici
    for (uint8_t loop = 0; loop <= 4; loop++)
    {
        Byte_COMMAND = _Calc_R_COMMAND(Registre);

        Wire.beginTransmission(_adress);
        Wire.write(Byte_COMMAND);
        Wire.endTransmission();

        Wire.requestFrom(_adress, byte(4));
        C0_LSB = Wire.read();
        C0_MSB = Wire.read();
        C1_LSB = Wire.read();
        C1_MSB = Wire.read();

        // Wire.requestFrom(_adress, byte(1));
        // switch (loop)
        // {
        // case 0:
        //     Registre = R_TSL2591_C0DATAH;

        //     break;
        // case 1:
        //     /* code */
        //     Registre = R_TSL2591_C1DATAL;
        //     C0_MSB = Wire.read();
        //     break;
        // case 2:
        //     /* code */
        //     Registre = R_TSL2591_C1DATAH;
        //     C1_LSB = Wire.read();
        //     break;
        // case 3:
        //     /* code */
        //     C1_MSB = Wire.read();
        //     break;

        // default:

        //     break;
        // }

        Wire.endTransmission();
    }

    delay(Valeur_time + 20); // delay ici 300 on va changer la valeur qu'on choisi en configuration
    disable();               // On place la fonction disable ici
    DATA = (((C0_MSB << 8) + C0_LSB) << 16) + ((C1_MSB << 8) + C1_LSB);

    _ALLData = DATA;
    // Serial.print("ALLDATA =");
    // Serial.println(_ALLData, BIN);

    return DATA;
}
