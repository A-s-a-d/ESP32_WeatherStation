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
    testln("_calculating command");
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
    teststart();

    uint8_t Byte_COMMAND = _Calc_R_COMMAND(R_TSL2591_ID);

    test_aff_txt_val_format("****CHECK ID ***** COMMAND is: 0b", Byte_COMMAND, BIN);

    Wire.beginTransmission(_adress);
    Wire.write(Byte_COMMAND);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    uint8_t ID = Wire.read();
    Wire.endTransmission();

    test_aff_txt_val_format("le ID est : 0x", ID, HEX);
    testend();
    return ID;
}

void TSL2591 ::config(uint8_t gain, uint8_t time)
{
    teststart();
    /* you have to send a the adresse of register by
    writing in the command  register */
    /* command register : 1 BYTE, 1bit CMD, 2bits transaction, 5 bits @ register
    CMD 1 when adressing a register,
    Transaction for different modes, 01 for normal modes
    adress:@ of register */
    uint8_t Byte_COMMAND = _Calc_R_COMMAND(R_TSL2591_CONFIG);

    test_aff_txt_val_format("****CHECK CONFIG*****  COMMAND is: 0b", Byte_COMMAND, BIN);

    _gain = gain;
    _time = time;

    uint8_t Byte_CONFIG = 0;

    test_aff_txt_val_format("gain est : 0X", gain, HEX);
    test_aff_txt_val_format("time est : 0X", time, HEX);
    test_aff_txt_val_format("_gain est : 0X", _gain, HEX);
    test_aff_txt_val_format("_time est : 0X", _time, HEX);

    /* Byte_CONFIG testé en Main, fonnctionne. */
    Byte_CONFIG = ((Byte_CONFIG + _gain) << 4) + _time;

    test_aff_txt_val_format("la configuration write est : 0b", Byte_CONFIG, BIN);

    Wire.beginTransmission(_adress);
    Wire.write(Byte_COMMAND);
    Wire.write(Byte_CONFIG);
    Wire.endTransmission();

    Wire.beginTransmission(_adress);
    Wire.write(Byte_COMMAND);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    uint8_t READ_CONFIG = Wire.read();
    test_aff_txt_val_format("la configuration read est : 0b", READ_CONFIG, BIN);
    Wire.endTransmission();
    testend();
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
    teststart();
    uint8_t Byte_COMMAND = _Calc_R_COMMAND(R_TSL2591_ENABLE);

    /* Byte_CONFIG testé en Main, fonnctionne. */
    test_aff_txt_val_format("****CHECK ENABLE***** COMMAND is: 0b", Byte_COMMAND, BIN);
    uint8_t Byte_Enable = 0;
    Byte_Enable = 0b00010011; // PON et AEN et AIEN activer
    test_aff_txt_val_format("byte Enable: 0b", Byte_Enable, BIN);

    Wire.beginTransmission(_adress);
    Wire.write(Byte_COMMAND);
    Wire.write(Byte_Enable);
    Wire.endTransmission();
    testln("TLS2591 activé");
    testend();
}

void TSL2591 ::disable()
{
    teststart();
    uint8_t Byte_COMMAND = _Calc_R_COMMAND(R_TSL2591_ENABLE);

    /* Byte_CONFIG testé en Main, fonnctionne. */

    test_aff_txt_val_format("****CHECK DISABLE****** COMMAND is: 0b", Byte_COMMAND, BIN);
    uint8_t Byte_Enable = 0;
    Byte_Enable = 0b00000000; // PON et AEN et AIEN desactiver
    test_aff_txt_val_format("byte Enable: 0b", Byte_Enable, BIN);

    Wire.beginTransmission(_adress);
    Wire.write(Byte_COMMAND);
    Wire.write(Byte_Enable);
    Wire.endTransmission();
    testln("TLS2591 desactivé");
    testend();
}

uint16_t TSL2591 ::getFullSpectrum() /* CH0 */
{
    teststart();
    test_aff_txt_val_format("DATA ALL est : 0b", _ALLData, BIN);
    test_aff_txt_val_format("****CHECK getFullSpectrum ***** Full Spectrum est  : 0b", ((_ALLData & 0XFFFF0000) >> 16), BIN);
    testend();
    return ((_ALLData & 0XFFFF0000) >> 16);
}

uint16_t TSL2591 ::getInfraRedSpectrum() /* CH1 */
{
    teststart();
    test_aff_txt_val_format("DATA ALL est : 0b", _ALLData, BIN);
    test_aff_txt_val_format("****CHECK getFullSpectrum ***** IR Spectrum est  : 0b", _ALLData & 0X0000FFFF, BIN);
    testend();
    return _ALLData & 0X0000FFFF;
}

uint16_t TSL2591 ::getVisibleSpectrum()
{
    teststart();
    test_aff_txt_val_format("DATA ALL est : 0b", _ALLData, BIN);
    uint16_t VisibleSpectrum = (getFullSpectrum() - getInfraRedSpectrum());
    test_aff_txt_val_format("****CHECK getFullSpectrum ***** Visible Spectrum est  : 0b", VisibleSpectrum, BIN);
    testend();
    return VisibleSpectrum;
}

float TSL2591 ::calculateLux() // change uint32_t to float later
{
    teststart();
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

        test_aff_txt_val_format("****CHECK calculateLux ***** cpl est  : 0b", cpl, BIN);
        test_aff_txt_val_format("****CHECK calculateLux ***** _time est  : 0b", _time, BIN);
        test_aff_txt_val_format("****CHECK calculateLux ***** _gain est  : 0b", _gain, BIN);

        lux = ((getVisibleSpectrum()) * (1 - getInfraRedSpectrum() / getFullSpectrum())) / cpl;

        test_aff_txt_val_format("****CHECK calculateLux ***** lux est  : ", lux, DEC);
        testend();
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
    teststart();

    uint8_t Registre = R_TSL2591_C0DATAL;
    uint8_t Byte_COMMAND = 0;
    uint8_t C0_LSB = 0;
    uint8_t C0_MSB = 0;
    uint8_t C1_LSB = 0;
    uint8_t C1_MSB = 0;
    uint32_t DATA = 0;
    testln("entery before loop");

    enable(); // On place la fonction enable ici
    for (uint8_t loop = 0; loop <= 4; loop++)
    {
        testln("entery inside loop");
        Byte_COMMAND = _Calc_R_COMMAND(Registre);
        test_aff_txt_val_format("****CHECK REGISTRE ALLDATAS*****le Registre Data est: 0x", Registre, HEX);
        test_aff_txt_val_format("****CHECK ALLDATAS*****  COMMAND 1 is is: 0b", Byte_COMMAND, BIN);

        Wire.beginTransmission(_adress);
        Wire.write(Byte_COMMAND);
        Wire.endTransmission();
        Wire.requestFrom(_adress, byte(1));

        switch (loop)
        {
        case 0:
            Registre = R_TSL2591_C0DATAH;
            C0_LSB = Wire.read();
            break;
        case 1:
            /* code */
            Registre = R_TSL2591_C1DATAL;
            C0_MSB = Wire.read();
            break;
        case 2:
            /* code */
            Registre = R_TSL2591_C1DATAH;
            C1_LSB = Wire.read();
            break;
        case 3:
            /* code */
            C1_MSB = Wire.read();
            break;

        default:
            testln("switch condition valeur defaut ou fin loop");
            break;
        }
        Wire.endTransmission();

        testln("@@@@@@@@@@@@@@next loop cycle @@@@@@@@@@@@@@");
        testln(" ");
    }

    delay(Valeur_time + 20); // delay ici 300 on va changer la valeur qu'on choisi en configuration
    disable();               // On place la fonction disable ici

    // C0_MSB = 0b10101010;
    // C0_LSB = 0b0;
    // C1_MSB = 0b10001101;
    // C1_LSB = 0b0;

    DATA = (((C0_MSB << 8) + C0_LSB) << 16) + ((C1_MSB << 8) + C1_LSB);

    test("C0_MSB C0_LSB C1_MSB C1_LSB : 0b ");
    test_multi_4_var_same_line_format(C0_MSB, C0_LSB, C1_MSB, C1_LSB, BIN);
    test_aff_txt_val_format("DATA ALL est : 0b", DATA, BIN);

    /* TESTE D'OPERATION DATA ^
        uint8_t C0_LSB = 0b11110000;
        uint8_t C0_MSB = 0b10101010;
        uint8_t C1_LSB = 0b11111111;
        uint8_t C1_MSB = 0b00000000;
        uint32_t DATA = 0;
        DATA = (((C0_MSB << 8) + C0_LSB) << 16) + ((C1_MSB << 8) + C1_LSB);
        delay(1000);
    */
    testend();
    _ALLData = DATA;
    // Serial.print("ALLDATA =");
    // Serial.println(_ALLData, BIN);

    return DATA;
}