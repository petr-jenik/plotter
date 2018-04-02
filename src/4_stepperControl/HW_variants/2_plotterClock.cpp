/*
 * 2_plotterClock.cpp
 *
 *  Created on: 24. 3. 2018
 *      Author: apollo
 */

#include "config.h"
#if PRINTER_TYPE == PRINTER_TYPE_PLOTTER_CLOCK

#include "mechanicController.h"
#include "global.h"

#include "hwStepperPins.h"
#include "servo.h"
#include "Timer.h"

#include "stepperControl_main.h"
#include "math_tools.h"

MechanicController mechanicController;

Servo servos[] =
{
		{0}, {1}, {2}
};


void stepperControl_init(void)
{
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
	 *          S1 S2
	 *
	 */

	// C = Required position

	position A;
	position B;
	position C;
	position D = finalPosition;

    //position A1,A2;
    //position B1,B2;
    //position C1,C2;


    bool result = true;
    position referencePoint;


    // Calculate position of the point B first
	referencePoint = (position){10, 0, 0} + pos_S2;
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
    referencePoint = (position){-10, 0, 0} + pos_S1;
    result = result and getIntersectionCloserToRefPoint(C,
											 armLength_AC,
											 pos_S1,
											 armLength_AS1,
											 referencePoint,  // move reference point to the rigth
											 A);

    //result = result and getIntersection(D, armLength_CD, B, armLength_BD, C1, C2);
    //result = result and getIntersection(finalPosition, armLength_AC, pos_S1, armLength_AS1, A1, A2);

    //result = result and getIntersection(finalPosition, armLength_AC, pos_S1, armLength_AS1, A1, A2);
    //result = result and getIntersection(finalPosition, armLength_BC, pos_S2, armLength_BS2, B1, B2);

    if (result)
    {
        //A = (A1.x < A2.x) ? A2 : A1;
    	//B = (B1.x < B2.x) ? B1 : B2;

    	float angle1 = getAngle(pos_S1, {pos_S1.x+100, pos_S1.y, pos_S1.z}, A);
    	float angle2 = getAngle(pos_S2, {pos_S2.x+100, pos_S2.y, pos_S2.z}, B);

    	// Fill servo setting
    	outputCmd.servoAngle[0] = angle1 - LEFT_ARM_OFFSET;
    	outputCmd.servoAngle[1] = angle2 - RIGHT_ARM_OFFSET;
    	outputCmd.servoAngle[2] = (extrudeLength > 0) ? 180 : 0;
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

#endif // PRINTER_TYPE == PRINTER_TYPE_PLOTTER_CLOCK
