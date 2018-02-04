/*
 * stepperController.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#include <plotterarmController.h>
#include "Timer.h"
#include "global.h"

#include "math_tools.h"

//StepperController::StepperController(IDatabase<sStepperSetting> &database)
ArmController::ArmController(PlotterArm & _arm_left, PlotterArm & _arm_right, Servo & _servo)
//ArmController::ArmController(PlotterArm & _stepper_left, PlotterArm & _stepper_right, Servo & _servo)
:armLeft(_arm_left),
armRight(_arm_right),
servo(_servo),
armObjectArray({&armLeft, &armRight}),
servoObjectArray({&servo}),
bCalibrationFinished(false)
{
	//this->stepperObjectArray = {stepper_left, stepper_right};
	//this->servoObjectArray = {servo};

    this->armObjectCount = 2;
    this->servoObjectCount = 1;
}

/*
//void StepperController::reg(void *object)
void StepperController::reg(void * object, eObjectType type)
{
    switch(type)
    {
    case eObjectType_stepper:
        this->stepperObjectArray[this->stepperObjectCount] = (Stepper*)object;
        this->stepperObjectCount++;
        break;

    case eObjectType_servo:
        this->servoObjectArray[this->servoObjectCount] = (Servo*)object;
        this->servoObjectCount++;
        break;

    default:
        break;
    }
}
*/

void ArmController::OnUpdateAll(armCommand command)
{
	//std::cout << std::endl;
	//std::cout << __FUNCTION__ << std::endl;
	//std::cout << command;
	// Test block
	//{
   // 	 float angle1 = command.angle1;
   // 	 float angle2 = command.angle2;
    	 //std::cout << "Angle 1: " << angle1 << ", angle 2: " << angle2 << std::endl;
	//}


	StepperSetting leftArm =  {command.angle1, true};
	StepperSetting rightArm = {command.angle2, true};
	ServoSetting servoSetting = {command.relPosZ, command.extrude };

	armLeft.OnUpdate(&leftArm);
	armRight.OnUpdate(&rightArm);
	servo.OnUpdate(&servoSetting);
/*
    for(int i = 0; i < this->stepperObjectCount; i++)
    {
        this->stepperObjectArray[i]->OnUpdate();
    }

    for(int i = 0; i < this->servoObjectCount; i++)
    {
        this->servoObjectArray[i]->OnUpdate();
    }
    */
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

/*
void * StepperController::getDatabase(void)
{
    return (void*)&this->database;
}*/

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
