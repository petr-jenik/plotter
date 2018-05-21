/*
 * 2d_plotter.cpp
 *
 *  Created on: 24. 3. 2018
 *      Author: apollo
 */

#include "project_config.h"
#if PRINTER_TYPE == PRINTER_TYPE_2D_PLOTTER_TYPE_SCARA_ROBOT

#include "mechanicController.h"
#include "global.h"

#include "hwStepperPins.h"
#include "servo.h"
#include "Timer.h"
#include "project_config.h"

#include "stepperControl_main.h"
#include "math_tools.h"

MechanicController mechanicController;

PlotterArm plotterArms[] =
{
		{{MIN_ANGLE, MAX_ANGLE, LEFT_ARM_OFFSET}},
		{{MIN_ANGLE, MAX_ANGLE, RIGHT_ARM_OFFSET}}
};

PlotterServo servos[] =
{
		{1}
};


void stepperControl_init(void)
{
	plotterArms[0].registerGPIOs(getStepperGPIOs(0));
	plotterArms[0].registerLimitSwitchGPIOs(getLimitSwitchGPIOs(0));

	plotterArms[1].registerGPIOs(getStepperGPIOs(1));
	plotterArms[1].registerLimitSwitchGPIOs(getLimitSwitchGPIOs(1));

	mechanicController.registerArms(&plotterArms[0]);
	mechanicController.registerArms(&plotterArms[1]);

	//mechanicController.registerArms(plotterArms, ARRAY_SIZE(plotterArms));
    mechanicController.registerServos(servos, ARRAY_SIZE(servos));

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
bool calculatePosition(position finalPosition, float extrudeLength, MechanicCommand& outputCmd)
{
	/*
	 *           C
	 *           /\
	 *          /  \
	 *         /    \
	 *        /      \
	 *       A        B
	 *        \      /
	 *         \    /
	 *          \  /
	 *          S1 S2
	 *
	 */

	// C = Required position

    position A1,A2;
    position B1,B2;

    bool result = true;

    result = result and getIntersection(finalPosition, armLength_AC, pos_S1, armLength_AS1, A1, A2);
    result = result and getIntersection(finalPosition, armLength_BC, pos_S2, armLength_BS2, B1, B2);

    if (result)
    {
        position A = (A1.x < A2.x) ? A2 : A1;
    	position B = (B1.x < B2.x) ? B1 : B2;

    	float angle1 = getAngle(pos_S1, {pos_S1.x+100, pos_S1.y, pos_S1.z}, A);
    	float angle2 = getAngle(pos_S2, {pos_S2.x+100, pos_S2.y, pos_S2.z}, B);

    	// Fill only arm positions
    	outputCmd.plotterArmAngle[0] = angle1;
    	outputCmd.plotterArmAngle[1] = angle2;
    	outputCmd.plotterArmObjectCount = 2;

    	// Fill servo setting
    	outputCmd.servoAngle[0] = (extrudeLength > 0) ? 180 : 0;
    	outputCmd.servoObjectCount = 1;

    	// Null other counts
		outputCmd.stepperObjectCount = 0;
		outputCmd.limStepperObjectCount = 0;
    	return true;
    }

    return false;
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
		LOG("Unable to navigate to this position!");
	}
}

#endif //PRINTER_TYPE == PRINTER_TYPE_2D_PLOTTER_TYPE_SCARA_ROBOT
