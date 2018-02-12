/*
 * mainApp.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#include <plotterarmController.h>
#include "myGlobal.h"

#include "database.h"
#include "hwStepperPins.h"
#include "servo.h"
#include "Timer.h"

#include "global.h"

#include "app_threads.h"

using namespace std;

//Communication communication;

const ArmConfig armConfigLeft = {MIN_ANGLE, MAX_ANGLE, LEFT_ARM_OFFSET};
const ArmConfig armConfigRight = {MIN_ANGLE, MAX_ANGLE, RIGHT_ARM_OFFSET};

PlotterArm stepperLeft(getStepperPins(0), armConfigLeft, getSwitchPins(0));
PlotterArm stepperRight(getStepperPins(1), armConfigRight, getSwitchPins(1));
Servo servo(1);

ArmController stepperController(stepperLeft, stepperRight, servo);

bool isSystemReady()
{
	return stepperController.calibrationFinished();
}


void stepperControl_init(void)
{
    stepperController.calibrate(true);

    while(!stepperController.calibrationFinished())
    {
    		stepperController.calibrate(false);
    }
}

void stepper_parseCommand(armCommand newCommand)
{
    stepperController.OnUpdateAll(newCommand);
    stepperController.OnMove();
}
