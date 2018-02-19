/*
 * hwStepperPins.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#include "hwStepperPins.h"
#include "myGlobal.h"
#include "stepperConfig.h"

sStepperPins stepperPinsUndef =
{
    undefPin, //directionPin
    undefPin, //stepPin
    undefPin, //nResetPin
    undefPin, //nSleepPin
    undefPin, //nEnablePin
};

sSwichPins limitPinsUndef =
{
        undefPin, // Limit switch 1
        undefPin, // Limit switch 2
};

sSwichPins limitPins1 =
{
        {eGPIO_PORT_D, 0 ,  true, eGPIO_Mode_Input, eGPIO_Pull_Up}, // Limit switch 1
        {eGPIO_PORT_C, 11 , true, eGPIO_Mode_Input, eGPIO_Pull_Up }, // Limit switch 2
};

sSwichPins limitPins2 =
{
        {eGPIO_PORT_E, 7 ,  true, eGPIO_Mode_Input, eGPIO_Pull_Up}, // Limit switch 1
        {eGPIO_PORT_E, 8 ,  true, eGPIO_Mode_Input, eGPIO_Pull_Up}, // Limit switch 2
};

sStepperPins stepperPins1 =
{
    {eGPIO_PORT_C, 13 , false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //directionPin
    {eGPIO_PORT_C, 15 , false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //stepPin
    {eGPIO_PORT_B, 5  , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nResetPin
    {eGPIO_PORT_C, 14 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No}, //nSleepPin
    {eGPIO_PORT_B, 8  , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nEnablePin
};

sStepperPins stepperPins2 =
{
    {eGPIO_PORT_E, 2 , false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //directionPin
    {eGPIO_PORT_E, 3 , false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //stepPin
    {eGPIO_PORT_E, 5 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nResetPin
    {eGPIO_PORT_E, 4 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nSleepPin
    {eGPIO_PORT_E, 6 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nEnablePin
};

sStepperPins stepperPins3 =
{
    {eGPIO_PORT_C, 9 ,  false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //directionPin
    {eGPIO_PORT_C, 8 ,  false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //stepPin
    {eGPIO_PORT_A, 9 ,  true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nResetPin
    {eGPIO_PORT_A, 8 ,  true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nSleepPin
    {eGPIO_PORT_A, 10 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nEnablePin
};

sStepperPins * stepperPins[eStepperMaxCount] =
   {&stepperPins1,
    &stepperPins2,
    &stepperPins3,
    &stepperPinsUndef
    };

sSwichPins * switchPins[eStepperMaxCount] =
   {&limitPins1,
    &limitPins2,
    &limitPinsUndef,
    &limitPinsUndef
    };


sStepperPins getStepperPins(int idx)
{

    if (0 <= idx && eStepperMaxCount > idx)
    {
        return * stepperPins[idx];
    }
    else
    {
        return stepperPinsUndef;
    }
}

sSwichPins getSwitchPins(int idx)
{

    if (0 <= idx && eStepperMaxCount > idx)
    {
        return * switchPins[idx];
    }
    else
    {
        return limitPinsUndef;
    }
}
