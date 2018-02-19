/*
 * stepperController.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#include <armController.h>
#include "Timer.h"
#include "global.h"

#include "math_tools.h"

ArmController::ArmController(PlotterArm & _armLeft, PlotterArm & _armRight, Servo & _servo)
:armLeft(_armLeft),
armRight(_armRight),
servo(_servo),
armObjectArray({&armLeft, &armRight}),
servoObjectArray({&servo}),
bCalibrationFinished(false)
{
    this->armObjectCount = 2;
    this->servoObjectCount = 1;
}


void ArmController::OnUpdateAll(armCommand command)
{
	// TODO Unify usage of steppers - use each of them or use an array

	StepperSetting leftArm =  {command.angle1, true};
	StepperSetting rightArm = {command.angle2, true};
	ServoSetting servoSetting = {command.relPosZ, (bool)command.extrudeLength };

	armLeft.OnUpdate(&leftArm);
	armRight.OnUpdate(&rightArm);
	servo.OnUpdate(&servoSetting);
}

int ArmController::GetMaxStepperError(void)
{
    int maxError = 0;

    for(int i = 0; i < this->armObjectCount; i++)
    {
        int error = abs(this->armObjectArray[i]->getError());
        maxError = (maxError < error)? error: maxError;
    }
    return maxError;
}

void ArmController::OnMove(void)
{
    // Update all servo motors
    for(int i = 0; i < this->servoObjectCount; i++)
    {
        this->servoObjectArray[i]->OnMove();
    }

    // Main logic for moving with steppers
    int maxError = this->GetMaxStepperError();

    for (int i = 0; i < maxError; i++)
    {
        for(int stepperIdx = 0; stepperIdx < this->armObjectCount; stepperIdx++)
        {
            this->armObjectArray[stepperIdx]->OnUpdateRegulation();
        }

        for(int i = 0; i < this->armObjectCount; i++)
        {
            this->armObjectArray[i]->moveStart();
        }

        Timer::sleep(2); //Sleep in ms

        for(int i = 0; i < this->armObjectCount; i++)
        {
            this->armObjectArray[i]->moveEnd();
        }

        //TODO maybe remove delay
        Timer::sleep(10); //Sleep in ms
    }
}

bool ArmController::calibrationFinished()
{
	return this->bCalibrationFinished;
}

void ArmController::calibrate(bool startNewCalibration)
{
	if (startNewCalibration)
	{
		this->bCalibrationFinished = false;
	}
    //bool calibrationFinished = false;

    if (false == this->bCalibrationFinished)
    {
        bool bIsCalibrationDone = true;
        for (int i = 0; i < this->armObjectCount; i++)
        {
            bool result = this->armObjectArray[i]->Calibrate();
            bIsCalibrationDone &= result;
        }

        this->bCalibrationFinished = bIsCalibrationDone;

        /*
		for(int idx = 0; idx < this->stepperObjectCount; idx++)
		{
			int actualValue = stepperObjectArray[idx]->actualValue;
			int setpoint = stepperObjectArray[idx]->setpointValue;
			int error = stepperObjectArray[idx]->getError();
			std::cout << "----------" << std::endl;
			std::cout << "Stepper[" << idx << "]" << std::endl;
			std::cout << "setpoint: " << setpoint << std::endl;
			std::cout << "actualValue: " << actualValue << std::endl;
			std::cout << "error: " << error << std::endl;
		}*/

        for(int i = 0; i < this->armObjectCount; i++)
        {
            this->armObjectArray[i]->moveStart();
        }

        Timer::sleep(2); //Sleep in ms

        for(int i = 0; i < this->armObjectCount; i++)
        {
            this->armObjectArray[i]->moveEnd();
        }

        //TODO maybe remove delay
        //Timer::sleep(10); //Sleep in ms
    }
}
