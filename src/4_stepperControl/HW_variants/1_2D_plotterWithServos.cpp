/*
 * 2_plotterClock.cpp
 *
 *  Created on: 24. 3. 2018
 *      Author: Apollo
 */

#include "project_config.h"
#if PRINTER_TYPE == PRINTER_TYPE_2D_PLOTTER_WITH_SERVOS

#include "mechanicController.h"
#include "global.h"

#include "hwStepperPins.h"
#include "servo.h"
#include "Timer.h"
#include "project_config.h"

#include "stepperControl_main.h"
#include "math_tools.h"

MechanicController mechanicController;

PlotterServo usedServos[] =
{
		{0, 61, false, -45, 180},
		{1, 140, true, -45, 180},
		{2, 0, false, 0, 180}
};


void stepperControl_init(void)
{
	TRACE; // Trace macro

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
bool calculatePosition(position finalPosition, float extrudeLength, MechanicCommand& outputCmd)
{
	TRACE; // Trace macro
	/*              D
	 *             /
	 *           C/
	 *           /\
	 *          /  \
	 *         /    \
	 *        /      \
	 *       A        B
	 *        \      /
	 *         \    /
	 *          \  /
	 *  right - S1 S2 - left
	 *
	 */

	// C = Required position

	position A;
	position B;
	position C;
	position D = finalPosition;

    bool result = true;
    position referencePoint;

    // Calculate position of the point B first
	referencePoint = (position){-10, 0, 0} + pos_S2;
    result = result and getIntersectionCloserToRefPoint(D,
														 armLength_BD,
														 pos_S2,
														 armLength_BS2,
														 referencePoint, // move reference point to the left const position referencePoint,
														 B);


    // Next, calculate position of the point C
    referencePoint = pos_S2;
    result = result and getIntersectionCloserToRefPoint(D,
											 armLength_CD,
											 B,
											 armLength_BC,
											 referencePoint,
											 C);

    // Next, calculate position of the point A
    referencePoint = (position){10, 0, 0} + pos_S1;
    result = result and getIntersectionCloserToRefPoint(C,
											 armLength_AC,
											 pos_S1,
											 armLength_AS1,
											 referencePoint,  // move reference point to the right
											 A);

    // Calculate position of the point B first
	referencePoint = (position){-10, 0, 0} + pos_S2;
    result = result and getIntersectionCloserToRefPoint(C,
														 armLength_BC,
														 pos_S2,
														 armLength_BS2,
														 referencePoint, // move reference point to the left const position referencePoint,
														 B);


    if (result)
    {
    	static float angle = 45;
    	//angle = 45 + (angle++)%45;
    	angle += 0.1;
    	if (angle > 90)
    	{
    		angle = 45;
    	}

    	float angleRight = getAngle(pos_S1, {pos_S1.x+100, pos_S1.y, pos_S1.z}, A);
    	float angleLeft =  180 - getAngle(pos_S2, {pos_S2.x+100, pos_S2.y, pos_S2.z}, B);

    	LOG("angleRight: " << angleRight << ", angleLeft" << angleLeft);

    	// Fill servo setting
    	outputCmd.servoAngle[eIdxRight] = angleRight;
    	outputCmd.servoAngle[eIdxLeft]  = angleLeft;

    	outputCmd.servoAngle[2] = (extrudeLength > 0) ? 0 : 180;
    	outputCmd.servoObjectCount = 3;

    	// Null other counts
    	outputCmd.plotterArmObjectCount = 0;
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


void stepperControl_goToThisPosition(position newPosition,
		                                 float extrudeLength,
										 float speed)
{
	TRACE; // Trace macro

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

#endif // PRINTER_TYPE == PRINTER_TYPE_2D_PLOTTER_WITH_SERVOS
