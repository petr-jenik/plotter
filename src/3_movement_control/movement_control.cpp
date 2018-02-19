/*
 * movement_control.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include "app_threads.h"
#include "math_tools.h"

#include "config.h"

#include <iostream>

#include "stepperControl.h"
#include "stepperConfig.h"

using namespace std;


position gCurrentPosition = {0,0,0};

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
bool createArmCommand(position C, armCommand& outputCmd)
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

    result = result and getIntersection(C, armLength_AC, pos_S1, armLength_AS1, A1, A2);
    result = result and getIntersection(C, armLength_BC, pos_S2, armLength_BS2, B1, B2);

    //std::cout << "Intersection of" << C << "and " << pos_S1 << pos_S2 << std::endl;
    if (result)
    {
        position A = (A1.x < A2.x) ? A2 : A1;
    	position B = (B1.x < B2.x) ? B1 : B2;

    	float angle1 = getAngle(pos_S1, {pos_S1.x+100, pos_S1.y}, A);
    	float angle2 = getAngle(pos_S2, {pos_S2.x+100, pos_S2.y}, B);

		//LOG("movementControl_loop: Angles: " << angle1 << "," << angle2);
    	//LOG("ANGLE1: " << angle1);
    	//LOG("ANGLE2: " << angle2);
    	//std::cout << "ANGLE1: " << angle1 << std::endl;
    	//std::cout << "ANGLE2: " << angle2 << std::endl;

    	//std::cout << "Req Angle 1: " << angle1 << ", Req angle 2: " << angle2 << std::endl;

    	// Fill only arm positions
    	// Extruder status will be filled in a different place
    	outputCmd.angle1 = angle1;
    	outputCmd.angle2 = angle2;
        outputCmd.relPosZ = zAxeToRelative(C.z);

    	return true;
    }

    return false;
}

#include "draw.h"

// TODO add speed
void sendArmCommand(position newPosition,float extrudeLength)
{
	armCommand armCmd;

	if (createArmCommand(newPosition, armCmd))
	{
		armCmd.extrudeLength = extrudeLength;
		stepper_parseCommand(armCmd);
		//cmdBuffer.send(armCmd);
	}
}


void movementControl_createLine(position finalPosition,
									float extrudeLength,
									float movementSpeed)
{
	position startPos = gCurrentPosition;

	//guiCommand cmd = {extrudeLength, movementSpeed, startPos, finalPosition};
	//gui_add_line(cmd, eColor_black);

	// get distance between start and end points
	float distance = getDistance3D(startPos, finalPosition);

	if (distance == 0)
	{
		LOG("distance = 0");
		return;
	}

	#define deltaX (finalPosition.x - startPos.x)
	#define deltaY	(finalPosition.y - startPos.y)
	#define deltaZ	(finalPosition.z - startPos.z)

	// A close approximation to a straight line between two points
	// TODO 13.10.2017 - This loop causes all the troubles!!!!!

	int numberOfSteps = (int)(SPEED_MAGICAL_CONSTANT*distance);

	// Do next loop at least once.
	if (0 == numberOfSteps)
	{
		numberOfSteps = 1;
	}

	LOG("numberOfSteps: " << numberOfSteps);

	for (float i = 0; i < numberOfSteps+1; i++)
	{
		position currentPos;
		currentPos.x = startPos.x + (deltaX * i)/numberOfSteps;
		currentPos.y = startPos.y + (deltaY * i)/numberOfSteps;
		currentPos.z = startPos.z + (deltaZ * i)/numberOfSteps;

		sendArmCommand(currentPos, extrudeLength / numberOfSteps);

		// TODO remove this - only for me to be able to see a difference between requested and actual position
		//guiCommand cmd = {extrudeLength / numberOfSteps, movementSpeed, gCurrentPosition, currentPos};
		//gui_add_line(cmd, eColor_green);

		gCurrentPosition = currentPos;
	}

	LOG("diff: " << gCurrentPosition - finalPosition);
}


void movementControl_createLine(const moveCommand& cmd)
{
	movementControl_createLine(cmd.finalPosition, cmd.extrudeLength, cmd.movementSpeed);
}



position p1 = {-100, 0, 0};
position p2 = {100, 0, 0};
position p3 = {10, 10, 0};
position p4 = {-10, 10, 0};

moveCommand demoCommands[] =
{
	{0, 1, p1},
	{1, 0.01, p2},
	{1, 0.01, p1},
	//{true, 0.01, p2, p3},
	/*
	{true, 0.01, p3, p4},
	{true, 0.01, p4, p1},
	{true, 0.01, p1, p3},
	{true, 0.01, p3, p1},
	{true, 0.01, p1, p2},
	{false, 0.01, p2, p4},
	{false, 0.01, p4, p2},
	{true, 0.01, p2, p1}
	*/
};

const int32_t demoCmdsCount = (sizeof(demoCommands)/sizeof(demoCommands[0]));

moveCommand getSqueatePoints(int idx, float size, position center)
{
	idx = (idx < 0)? 0 : idx;
	idx = (idx > 4)? 4 : idx;

	float speed = 1;
	float extrudeLength = size;

	moveCommand squareCommands[] =
	{
		{0 /*extrudeLength*/, speed, {center.x - size/2, center.y - size/2}},
		{extrudeLength, speed, {center.x + size/2, center.y - size/2}},
		{extrudeLength, speed, {center.x + size/2, center.y + size/2}},
		{extrudeLength, speed, {center.x - size/2, center.y + size/2}},
		{extrudeLength, speed, {center.x - size/2, center.y - size/2}}
	};

	return squareCommands[idx];
}


void printLine(position startPos, position endPos)
{
	moveCommand cmd = {0, 1 , startPos};
	movementControl_createLine(cmd);
    cmd = {1, 1 , endPos};
	movementControl_createLine(cmd);
}

void printRectangle(float size, position center)
{

	for (int idx = 0; idx < 5; idx++)
	{
		moveCommand cmd = getSqueatePoints(idx, size, center);
		movementControl_createLine(cmd);
	}
}


void printCircle(float radius, position center)
{
	int numberOfSteps = 36;
	for (int i = 0; i < numberOfSteps + 1; i++)
	{
		float angle = (float)(360 * i) / (float)(numberOfSteps);
		position pos = getCirclePosition(center, radius, angle);

		moveCommand cmd = {(i == 0)?0.0:1.0 /*extrudeLength*/, 1 /*speed*/, pos};
		movementControl_createLine(cmd);
	}
}


void demoReceive(moveCommand &cmd)
{
	static int32_t index = 0;

	static int32_t Y = 0;

	moveCommand array[2] = { {1,0.01, {-100,Y, 0}}, {1, 0.01, {100, Y, 0}}};

	cmd = array[index];

	index++;
	if (index > 1)
	{
		index= 0;
		Y--;
	}
	//index = (index + 1) % demoCmdsCount;
	//cmd = demoCommands[index];
	//index = (index + 1) % demoCmdsCount;
}


void uglyHack_setAngle()
{
	armCommand armCmd;
	static float angle = 90;
	static float step = 1;
	if (angle > 180)
	{
		step = -1;
	}

	if (angle < 0)
	{
		step = +1;
	}

	LOG("Requested angle: " << angle);

	angle += step;

	armCmd.angle1 = angle;
	armCmd.angle2 = 180-angle;
	//armCmd.relativeAngle1 = angleToRelative(angle);
	//armCmd.relativeAngle2 = angleToRelative(0);
	armCmd.extrudeLength = 1;
	stepper_parseCommand(armCmd);

	return;
}

void uglyHack_setPosition(void)
{
	moveCommand cmd;
	demoReceive(cmd);
	movementControl_createLine(cmd);
}

void showDemo()
{

//printRectangle(10, {0, 0, 0});

for (int i = 0; i < 10; i += 2)
{
	printRectangle(2*i, {0,0,0});
	//printLine({-150, i, 0}, {150, i, 0});
	//printCircle(i, { 0, 0, 0});
}

/*
	printCircle(10, {-100, 100, 0});
	printCircle(10, {-100, -100, 0});
	printCircle(10, {100, -100, 0});
	*/
}

void debug_loop(void)
{
	//auto delay = std::chrono::milliseconds(1000);
	//std::this_thread::sleep_for(delay);
	//uglyHack_setPosition();
	showDemo();
}


void movementControl_init()
{
	// Go to position {0, 0, 0}
	sendArmCommand({0, 0, 0}, 0);
}
