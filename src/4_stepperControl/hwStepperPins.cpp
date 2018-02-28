/*
 * hwStepperPins.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#include "global.h"
#include "hwStepperPins.h"
#include "stepperConfig.h"
#include "config.h"


sStepperPins stepperPins3 =
{
	{eGPIO_PORT_C, 9 ,  false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //directionPin
	{eGPIO_PORT_C, 8 ,  false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //stepPin
	{eGPIO_PORT_A, 9 ,  true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nResetPin
	{eGPIO_PORT_A, 8 ,  true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nSleepPin
	{eGPIO_PORT_A, 10 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nEnablePin
};

StepperGPIOs stepperGPIOs1(
		{
			{eGPIO_PORT_C, 13 , false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //directionPin
			{eGPIO_PORT_C, 15 , false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //stepPin
			{eGPIO_PORT_B, 5  , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nResetPin
			{eGPIO_PORT_C, 14 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No}, //nSleepPin
			{eGPIO_PORT_B, 8  , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nEnablePin
		},
		{
			{eGPIO_PORT_D, 0 ,  true, eGPIO_Mode_Input, eGPIO_Pull_Up}, // Limit switch 1
			{eGPIO_PORT_C, 11 , true, eGPIO_Mode_Input, eGPIO_Pull_Up }, // Limit switch 2
		}
);

StepperGPIOs stepperGPIOs2(
{
	{eGPIO_PORT_E, 2 , false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //directionPin
	{eGPIO_PORT_E, 3 , false, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //stepPin
	{eGPIO_PORT_E, 5 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nResetPin
	{eGPIO_PORT_E, 4 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nSleepPin
	{eGPIO_PORT_E, 6 , true,  eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No }, //nEnablePin
},
{
	{eGPIO_PORT_E, 7 ,  true, eGPIO_Mode_Input, eGPIO_Pull_Up}, // Limit switch 1
	{eGPIO_PORT_E, 8 ,  true, eGPIO_Mode_Input, eGPIO_Pull_Up}, // Limit switch 2
});

StepperGPIOs* getStepperGPIOs(int idx)
{

	StepperGPIOs * stepperGPIOs[] =
	{
		&stepperGPIOs1,
		&stepperGPIOs2
	};

	const int cStepperMaxCount = sizeof(stepperGPIOs)/sizeof(stepperGPIOs[0]);

    if (0 <= idx && cStepperMaxCount > idx)
    {
        return stepperGPIOs[idx];
    }
    else
    {
		LOG("Invalid stepper index!");
    	assert(false);
    	return stepperGPIOs[0];
    }
}
