/*
 * stepperController.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#include "mechanicController.h"
#include "building_blocks.h"
#include "Timer.h"
#include "global.h"

#include "math_tools.h"
#include <cstdlib>
#include "stepperConfig.h"

#define SET_EACH_MEMBER_TO_NULL(X) 	for (size_t i =0; i < ARRAY_SIZE((X));i++){(X)[i] = NULL;}


MechanicController::MechanicController()
:bCalibrationFinished(false),
armObjectCount(0),
stepperObjectCount(0),
limStepperObjectCount(0),
servoObjectCount(0)
{
	SET_EACH_MEMBER_TO_NULL(armObjectArray);
	SET_EACH_MEMBER_TO_NULL(stepperObjectArray);
	SET_EACH_MEMBER_TO_NULL(limStepperObjectArray);
	SET_EACH_MEMBER_TO_NULL(servoObjectArray);
};


void MechanicController::registerArms(PlotterArm * pArms)//, int armsCount)
{
	if ((pArms != NULL) and (armObjectCount <= cMaxNumberOfPlotterArms))
	{
		armObjectArray[armObjectCount++] = pArms;
	}
	else
	{
		new_assert(false);
	}
}


void MechanicController::registerLimSteppers(StepperWithLimits * pLimSteppers, int cSteppersCount)
{
	if ((pLimSteppers != NULL) and (cSteppersCount <= cNumberOfSteppersWithLimit))
	{
		for (int i = 0; i < cSteppersCount; i++)
		{
			limStepperObjectArray[i] = &(pLimSteppers[i]);
		}
		limStepperObjectCount = cSteppersCount;
	}
	else
	{
		new_assert(false);
	}
}


void MechanicController::registerServos(Servo * pServos, int servosCount)
{
	if ((pServos != NULL) and (servosCount <= cMaxNumberOfServos))
	{
		for (int i = 0; i < servosCount; i++)
		{
			servoObjectArray[i] = &(pServos[i]);
		}
		servoObjectCount = servosCount;
	}
	else
	{
		new_assert(false);
	}
}


void MechanicController::registerSteppers(Stepper * pSteppers, int steppersCount)
{
	if ((pSteppers != NULL) and (steppersCount <= cMaxNumberOfSteppers))
	{
		for (int i = 0; i < steppersCount; i++)
		{
			// TODO  fix this horrible solution - use pointer to pointere
			stepperObjectArray[i] = &(pSteppers[i]);
		}
		stepperObjectCount = steppersCount;
	}
	else
	{
		new_assert(false);
	}
}

void MechanicController::OnUpdateAll(MechanicCommand command)
{
	// TODO Unify usage of steppers - use each of them or use an array

	new_assert(command.servoObjectCount == this->servoObjectCount);
	new_assert(command.stepperObjectCount == this->stepperObjectCount);
	new_assert(command.limStepperObjectCount == this->limStepperObjectCount);
	new_assert(command.plotterArmObjectCount == this->armObjectCount);

	// Servos
	for (int i = 0; i < command.servoObjectCount; i++)
	{
		ServoSetting servoSetting = {command.servoAngle[i], true};
		servoObjectArray[i]->OnUpdate(servoSetting);
	}

	// Basic steppers
	for (int i = 0; i < command.stepperObjectCount; i++)
	{
		stepperObjectArray[i]->OnUpdate(command.stepperNumberOfStepps[i], true);
	}

	// Steppers with limit switches
	for (int i = 0; i < command.limStepperObjectCount; i++)
	{
		limStepperObjectArray[i]->OnUpdate(command.limStepperRelativePosition[i], true);
	}

	// Plotter Arms (special kind of steppers with limit switches)
	for (int i = 0; i < command.plotterArmObjectCount; i++)
	{
		PlotterArmSetting setting = {command.plotterArmAngle[i], true};
		armObjectArray[i]->NewPosition(setting); // left
	}
}


int MechanicController::GetMaxStepperError(void)
{
    int maxError = 0;

	for(int idx = 0; idx < this->armObjectCount; idx++)
	{
	    int error = abs(this->armObjectArray[idx]->getError());
        maxError = (maxError < error)? error: maxError;
	}

	for(int idx = 0; idx < this->stepperObjectCount; idx++)
	{
	    int error = abs(this->stepperObjectArray[idx]->getError());
        maxError = (maxError < error)? error: maxError;
	}

	for(int idx = 0; idx < this->limStepperObjectCount; idx++)
	{
	    int error = abs(this->limStepperObjectArray[idx]->getError());
        maxError = (maxError < error)? error: maxError;
	}
    return maxError;
}

void MechanicController::updateRegulationOnAll(void)
{
	// Update regulation on all object of type stepper or similar
	for(int idx = 0; idx < this->armObjectCount; idx++)
	{
		this->armObjectArray[idx]->OnUpdateRegulation();
	}

	for(int idx = 0; idx < this->stepperObjectCount; idx++)
	{
		this->stepperObjectArray[idx]->OnUpdateRegulation();
	}

	for(int idx = 0; idx < this->limStepperObjectCount; idx++)
	{
		this->limStepperObjectArray[idx]->OnUpdateRegulation();
	}
}


void MechanicController::moveWithEach(void)
{
	// Start step pulse
    for(int idx = 0; idx < this->armObjectCount; idx++)
    {
        this->armObjectArray[idx]->moveStart();
    }

    for(int idx = 0; idx < this->stepperObjectCount; idx++)
    {
        this->stepperObjectArray[idx]->moveStart();
    }

    for(int idx = 0; idx < this->limStepperObjectCount; idx++)
    {
        this->limStepperObjectArray[idx]->moveStart();
    }

    Timer::sleep(2); //Sleep in ms

    // End step pulse
    for(int idx = 0; idx < this->armObjectCount; idx++)
    {
        this->armObjectArray[idx]->moveEnd();
    }

    for(int idx = 0; idx < this->stepperObjectCount; idx++)
    {
        this->stepperObjectArray[idx]->moveEnd();
    }

    for(int idx = 0; idx < this->limStepperObjectCount; idx++)
    {
        this->limStepperObjectArray[idx]->moveEnd();
    }

    //TODO maybe remove delay
    Timer::sleep(10); //Sleep in ms
}

void MechanicController::OnMove(void)
{
    // Update all servo motors
    for(int i = 0; i < this->servoObjectCount; i++)
    {
        this->servoObjectArray[i]->OnMove();
    }
    //{&servo}),
    // Main logic for moving with steppers
    int maxError = this->GetMaxStepperError();

    for (int i = 0; i < maxError; i++)
    {
    	this->updateRegulationOnAll();
    	this->moveWithEach();
    }
}

bool MechanicController::calibrationFinished()
{
	return this->bCalibrationFinished;
}

void MechanicController::calibrate(bool startNewCalibration)
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

        for (int i = 0; i < this->limStepperObjectCount; i++)
        {
            bool result = this->limStepperObjectArray[i]->Calibrate();
            bIsCalibrationDone &= result;
        }

        this->bCalibrationFinished = bIsCalibrationDone;

    	this->moveWithEach();
/*
        for(int i = 0; i < this->armObjectCount; i++)
        {
            this->armObjectArray[i]->moveStart();
        }

        Timer::sleep(2); //Sleep in ms

        for(int i = 0; i < this->armObjectCount; i++)
        {
            this->armObjectArray[i]->moveEnd();
        }
*/
        //TODO maybe remove delay
        //Timer::sleep(10); //Sleep in ms
    }
}
