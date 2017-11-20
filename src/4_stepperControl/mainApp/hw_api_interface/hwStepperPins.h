/*
 * hwStepperPins.h
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#ifndef HWAPI_HWSTEPPERPINS_H_
#define HWAPI_HWSTEPPERPINS_H_

#include "hwGpio.h"

extern GpioDesc_t undefPin;

/*
typedef enum
{
    eStepperPin_Direction = 0,
    eStepperPin_Step,
    eStepperPin_nReset,
    eStepperPin_nSleep,
    eStepperPin_nEnable,

    eStepperPin_count
} sStepperPin;
*/


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

sSwichPins getSwitchPins(int idx);

#endif /* HWAPI_HWSTEPPERPINS_H_ */
