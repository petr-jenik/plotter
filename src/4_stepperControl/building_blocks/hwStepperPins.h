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


class StepperGPIOs
{
public:
	Gpio directionPin;
	Gpio enablePin;
	Gpio resetPin;
	Gpio sleepPin;
	Gpio stepPin;

	StepperGPIOs(sStepperPins stepperPinsDescription)
	:directionPin(stepperPinsDescription.directionPinDesc),
	enablePin(stepperPinsDescription.enablePinDesc),
	resetPin(stepperPinsDescription.resetPinDesc),
	sleepPin(stepperPinsDescription.sleepPinDesc),
	stepPin(stepperPinsDescription.stepPinDesc)
	{};
};

class LimitSwitchGPIOs
{
public:
	Gpio switchPin1;
	Gpio switchPin2;

	LimitSwitchGPIOs(sSwichPins switchPinsDescription)
	:switchPin1(switchPinsDescription.limitSwitch1),
	switchPin2(switchPinsDescription.limitSwitch2)
	{};
};

StepperGPIOs* getStepperGPIOs(int idx);
LimitSwitchGPIOs* getLimitSwitchGPIOs(int idx);
Gpio* getSingleLimitSwitchGPIO(int idx);

#endif /* HWAPI_HWSTEPPERPINS_H_ */
