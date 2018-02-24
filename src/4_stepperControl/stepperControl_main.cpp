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

//Communication communication;

const ArmConfig armConfigLeft = {MIN_ANGLE, MAX_ANGLE, LEFT_ARM_OFFSET};
const ArmConfig armConfigRight = {MIN_ANGLE, MAX_ANGLE, RIGHT_ARM_OFFSET};

PlotterArm stepperLeft(getStepperPins(0), armConfigLeft, getLimitSwitchPins(0));
PlotterArm stepperRight(getStepperPins(1), armConfigRight, getLimitSwitchPins(1));
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

#include "math_tools.h"

bool getNewPosition(armCommand newCommand, position &newPosition)
{
    position A = getCirclePosition(pos_S1, armLength_AC, newCommand.angle1);// - LEFT_ARM_OFFSET);
    position B = getCirclePosition(pos_S2, armLength_BC, newCommand.angle2);// - RIGHT_ARM_OFFSET);
	position C1, C2;
	position C;

	if (getIntersection(A, armLength_AC, B, armLength_BC, C1, C2))
	{
		float distC1 = getDistance(C1, pos_S1);
		float distC2 = getDistance(C2, pos_S2);

		C = (distC1 < distC2)? C2 : C1;
		newPosition = C;
		return true;
	}
	else
	{
		LOG("Intersection not found. A: " << A << ", B:" << B);
		return false;
	}
}

void stepperControl_parseCommand(armCommand newCommand)
{
	// TODO remove this - only for me to be able to see a difference between requested and actual position

	static position currentPosition = {0, 0, 0};
	position newPosition = {0, 0, 0};

	if(getNewPosition(newCommand, newPosition))
	{
		LOG("New position according to stepperControl" << currentPosition);
		currentPosition = newPosition;
	}

    stepperController.OnUpdateAll(newCommand);
    stepperController.OnMove();
}
