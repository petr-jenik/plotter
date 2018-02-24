/*
 * hwStepperPins.h
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#ifndef HWAPI_HWSTEPPERPINS_H_
#define HWAPI_HWSTEPPERPINS_H_

#include "hwGpio.h"

typedef struct
{
    GpioDesc_t directionPinDesc;
    GpioDesc_t stepPinDesc;
    GpioDesc_t resetPinDesc;
    GpioDesc_t sleepPinDesc;
    GpioDesc_t enablePinDesc;
} sStepperPins;

typedef struct
{
    GpioDesc_t limitSwitch1;
    GpioDesc_t limitSwitch2;
} sSwichPins;

sStepperPins getStepperPins(int idx);

sSwichPins getLimitSwitchPins(int idx);

#endif /* HWAPI_HWSTEPPERPINS_H_ */
