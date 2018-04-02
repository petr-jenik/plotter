/*
 * 3_3D_printer.cpp
 *
 *  Created on: 24. 3. 2018
 *      Author: apollo
 */


#include "config.h"
#if PRINTER_TYPE == PRINTER_TYPE_3D_PRINTER

#include "mechanicController.h"
#include "global.h"

#include "hwStepperPins.h"
#include "servo.h"
#include "Timer.h"

#include "stepperControl_main.h"
#include "math_tools.h"

MechanicController mechanicController;

StepperWithLimits steppers[eStepperIdx_COUNT];

void stepperControl_init(void)
{
	for (int i = 0; i < ARRAY_SIZE(steppers); i++)
	{
		LimitSwitchGPIOs* pLimSwitchGPIOs = getLimitSwitchGPIOs(i);
		new_assert(pLimSwitchGPIOs != NULL);
		steppers[i].registerLimitSwitchGPIOs(pLimSwitchGPIOs);

		StepperGPIOs * pStepperGPIOs = getStepperGPIOs(i);
		new_assert(pStepperGPIOs != NULL);
		steppers[i].registerGPIOs(pStepperGPIOs);
	}

	// Register steppers to the controller
	mechanicController.registerLimSteppers(steppers, ARRAY_SIZE(steppers));

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
bool calculatePosition(position D, float extrudeLength, MechanicCommand& outputCmd)
{
    bool result = true;
    result = result and isInRange(D.x, pos_START.x, pos_END.x);
    result = result and isInRange(D.y, pos_START.y, pos_END.y);
    result = result and isInRange(D.z, pos_START.z, pos_END.z);

    if(!result)
    {
    	LOG("Required position (pos: " << D << ") is not in a valid range.");
    	return false;
    }

    float relativeX = map(D.x, pos_START.x, pos_END.x, 0.0f, 1.0f);
    float relativeY = map(D.y, pos_START.y, pos_END.y, 0.0f, 1.0f);
    float relativeZ = map(D.z, pos_START.z, pos_END.z, 0.0f, 1.0f);

	// Fill only steppers with limits
	outputCmd.limStepperRelativePosition[eStepperIdx_X] = relativeX;
	outputCmd.limStepperRelativePosition[eStepperIdx_Y] = relativeY;
	outputCmd.limStepperRelativePosition[eStepperIdx_Z] = relativeZ;
	outputCmd.limStepperObjectCount = 3;

	// Null other counts
	outputCmd.servoObjectCount = 0;
	outputCmd.plotterArmObjectCount = 0;
	outputCmd.stepperObjectCount = 0;
	return true;
}

bool isSystemReady()
{
	return mechanicController.calibrationFinished();
}


// TODO add speed
void stepperControl_goToThisPosition(position newPosition,float extrudeLength)
{
	MechanicCommand command = {0};

	if (calculatePosition(newPosition, extrudeLength, command))
	{
	    mechanicController.OnUpdateAll(command);
	    mechanicController.OnMove();
	}
	else
	{
		LOG("Unable to navigate to this position!");
	}
}

#endif //PRINTER_TYPE == PRINTER_TYPE_3D_PRINTER



