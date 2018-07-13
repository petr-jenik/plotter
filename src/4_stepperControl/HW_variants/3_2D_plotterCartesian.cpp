/*
 * 3_2D_plotterCartesian.cpp
 *
 *  Created on: 21. 5. 2018
 *      Author: Apollo
 */


#include "project_config.h"
#if PRINTER_TYPE == PRINTER_TYPE_2D_PLOTTER_CARTESIAN

#include "mechanicController.h"
#include "global.h"

#include "hwStepperPins.h"
#include "servo.h"
#include "Timer.h"

#include "stepperControl_main.h"
#include "math_tools.h"

MechanicController mechanicController;

//StepperWithLimits steppers[eStepperIdx_COUNT];
//StepperWithOneLimitSwitch steppers[eStepperIdx_COUNT] = {1000, 1000};
StepperWithOneLimitSwitch stepperX(1000);
StepperWithOneLimitSwitch stepperY(1000);

StepperWithLimits * pSteppers[] = {&stepperX, &stepperY};


// Following values were determined using experimental methods (trial & fail)
PlotterServo usedServos[] = {
		{1/*channel*/,
		 180/*offset*/,
		 true/*reverse*/,
		 50/*min angle*/,
		 65/*max angle*/}
};

void stepperControl_init(void)
{
	Gpio* pLimSwitchGPIO_X = getSingleLimitSwitchGPIO(0);
	new_assert(pLimSwitchGPIO_X != NULL);
	stepperX.registerSingleLimitSwitchGPIO(pLimSwitchGPIO_X);

	StepperGPIOs * pStepperGPIOs = getStepperGPIOs(0);
	new_assert(pStepperGPIOs != NULL);
	stepperX.registerGPIOs(pStepperGPIOs);

	Gpio* pLimSwitchGPIO_Y = getSingleLimitSwitchGPIO(1);
	new_assert(pLimSwitchGPIO_Y != NULL);
	stepperY.registerSingleLimitSwitchGPIO(pLimSwitchGPIO_Y);

	pStepperGPIOs = getStepperGPIOs(1);
	new_assert(pStepperGPIOs != NULL);
	stepperY.registerGPIOs(pStepperGPIOs);

	// Register steppers to the controller

	mechanicController.registerLimSteppers(pSteppers ,ARRAY_SIZE(pSteppers));

	// Register servomotors to the controller
    mechanicController.registerServos(usedServos, ARRAY_SIZE(usedServos));

    mechanicController.calibrate(true);

    while(!mechanicController.calibrationFinished())
    {
    		mechanicController.calibrate(false);
    }

    LOG("Calibration finished!");
}


/* Creates control command for motor
 *
 * Finds required intersections and sets arms to required positions C
 *
 * @param[in]  C           required position
 * @param[out] outputCmd   result
 * @return     bool        True if intersection was found, False if not
 * out - arm command for motor
 *
 */
bool calculatePosition(position requiredPos, float extrudeLength, MechanicCommand& outputCmd)
{
    bool result = true;
    result = result and isInRange(requiredPos.x, pos_START.x, pos_END.x);
    result = result and isInRange(requiredPos.y, pos_START.y, pos_END.y);
    //result = result and isInRange(requiredPos.z, pos_START.z, pos_END.z);

    if(!result)
    {
    	DBG("Required position (pos: " << requiredPos << ") is not in a valid range.");
    	return false;
    }

    float relativeX = templateMap(requiredPos.x, pos_START.x, pos_END.x, 0.0f, 1.0f);
    float relativeY = templateMap(requiredPos.y, pos_START.y, pos_END.y, 0.0f, 1.0f);
    //float relativeZ = templateMap(requiredPos.z, pos_START.z, pos_END.z, 0.0f, 1.0f);

	// Fill only steppers with limits
	outputCmd.limStepperRelativePosition[eStepperIdx_X] = relativeX;
	outputCmd.limStepperRelativePosition[eStepperIdx_Y] = relativeY;
	//outputCmd.limStepperRelativePosition[eStepperIdx_Z] = relativeZ;
	outputCmd.limStepperObjectCount = 2;

	// Control Z axe with a servo
	outputCmd.servoAngle[0] = (extrudeLength > 0) ? 0 : 180;
	outputCmd.servoObjectCount = 1;

	// Null other counts
	outputCmd.plotterArmObjectCount = 0;
	outputCmd.stepperObjectCount = 0;
	return true;
}

bool isSystemReady()
{
	return mechanicController.calibrationFinished();
}


// TODO add speed
void stepperControl_goToThisPosition(position newPosition,
		                             float extrudeLength,
									 float speed)
{
	MechanicCommand command = {0};

	if (calculatePosition(newPosition, extrudeLength, command))
	{
	    mechanicController.OnUpdateAll(command);
	    mechanicController.OnMove();
	}
	else
	{
		DBG("Unable to navigate to this position!");
	}
}

#endif //PRINTER_TYPE == PRINTER_TYPE_3D_PRINTER_CARTESIAN



