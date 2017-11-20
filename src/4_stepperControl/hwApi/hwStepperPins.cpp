/*
 * hwStepperPins.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#include "hwStepperPins.h"
#include "myGlobal.h"
#include "stepperConfig.h"
#include "config.h"

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
        {1, ePin_LimSw1}, // Limit switch 1
        {1, ePin_LimSw2}, // Limit switch 2
};

sSwichPins limitPins2 =
{
        {2, ePin_LimSw1}, // Limit switch 1
        {2, ePin_LimSw2}, // Limit switch 2
};

sStepperPins stepperPins1 =
{
    {1, ePin_Dir }, //directionPin
    {1, ePin_Step }, //stepPin
    {1, ePin_Reset}, //nResetPin
    {1, ePin_Sleep }, //nSleepPin
    {1, ePin_Enable }, //nEnablePin
};

sStepperPins stepperPins2 =
{
    {2, ePin_Dir }, //directionPin
    {2, ePin_Step }, //stepPin
    {2, ePin_Reset}, //nResetPin
    {2, ePin_Sleep }, //nSleepPin
    {2, ePin_Enable }, //nEnablePin
};

/*
sStepperPins stepperPins2 =
{
    {eGPIO_PORT_E, 2 , false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //directionPin
    {eGPIO_PORT_E, 3 , false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //stepPin
    {eGPIO_PORT_E, 5 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nResetPin
    {eGPIO_PORT_E, 4 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nSleepPin
    {eGPIO_PORT_E, 6 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nEnablePin
};
*/

sStepperPins stepperPins3 =
{
    {3, ePin_Dir }, //directionPin
    {3, ePin_Step }, //stepPin
    {3, ePin_Reset}, //nResetPin
    {3, ePin_Sleep }, //nSleepPin
    {3, ePin_Enable }, //nEnablePin
};

/*
sStepperPins stepperPins3 =
{
    {eGPIO_PORT_C, 9 ,  false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //directionPin
    {eGPIO_PORT_C, 8 ,  false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //stepPin
    {eGPIO_PORT_A, 9 ,  false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nResetPin
    {eGPIO_PORT_A, 8 ,  true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nSleepPin
    {eGPIO_PORT_A, 10 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nEnablePin
};
*/

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
