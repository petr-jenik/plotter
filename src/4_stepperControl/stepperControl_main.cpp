/*
 * mainApp.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#include "global.h"

#include "armController.h"

#include "hwStepperPins.h"
#include "servo.h"
#include "Timer.h"
#include "config.h"

#include "stepperControl_main.h"

PlotterArm stepperLeft({MIN_ANGLE, MAX_ANGLE, LEFT_ARM_OFFSET});
PlotterArm stepperRight({MIN_ANGLE, MAX_ANGLE, RIGHT_ARM_OFFSET});
Servo servo(1);

ArmController stepperController(stepperLeft, stepperRight, servo);

bool isSystemReady()
{
	return stepperController.calibrationFinished();
}


void stepperControl_init(void)
{
	stepperLeft.registerGPIOs(getStepperGPIOs(0));
	stepperRight.registerGPIOs(getStepperGPIOs(1));

    stepperController.calibrate(true);

    while(!stepperController.calibrationFinished())
    {
    		stepperController.calibrate(false);
    }
}


void stepperControl_parseCommand(armCommand newCommand)
{
    stepperController.OnUpdateAll(newCommand);
    stepperController.OnMove();
}
