/*
 * movement_control.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include "math_tools.h"
#include "config.h"
#include "stepperConfig.h"

#include "stepperControl_main.h"
#include "movementControl_main.h"

// TODO remove next include - my_gui.h
#include "my_gui.h"

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

    	float angle1 = getAngle(pos_S1, {pos_S1.x+100, pos_S1.y, pos_S1.z}, A);
    	float angle2 = getAngle(pos_S2, {pos_S2.x+100, pos_S2.y, pos_S2.z}, B);

		std::cout << "movementControl_loop: Angles: " << 180 - angle1 << "," << angle2 << std::endl;
		std::cout << "diff: " << 180 - angle1 - angle2 << std::endl;
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

// TODO add speed
void sendArmCommand(position newPosition,float extrudeLength)
{
	armCommand armCmd;

	if (createArmCommand(newPosition, armCmd))
	{
		armCmd.extrudeLength = extrudeLength;
		stepperControl_parseCommand(armCmd);
		//cmdBuffer.send(armCmd);
	}
}


void movementControl_createLine(position finalPosition,
									float extrudeLength,
									float movementSpeed)
{
	position startPos = gCurrentPosition;

	// TODO remove next line
	guiCommand cmd = {1,1,startPos, finalPosition};
	gui_add_line(cmd, eColor_green);

	// get distance between start and end points
	float distance = getDistance3D(startPos, finalPosition);

	if (distance == 0)
	{
		LOG("distance = 0");
		return;
	}

	#define deltaX (finalPosition.x - startPos.x)
	#define deltaY (finalPosition.y - startPos.y)
	#define deltaZ (finalPosition.z - startPos.z)

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
		gCurrentPosition = currentPos;
	}
	LOG("diff: " << gCurrentPosition - finalPosition);
}


void movementControl_createLine(const moveCommand& cmd)
{
	movementControl_createLine(cmd.finalPosition, cmd.extrudeLength, cmd.movementSpeed);
}


moveCommand getSqueatePoints(int idx, float size, position center)
{
	idx = (idx < 0)? 0 : idx;
	idx = (idx > 4)? 4 : idx;

	float speed = 1;
	float extrudeLength = size;

	moveCommand squareCommands[] =
	{
		// First moveCommand simulates "move to" command
		{0            , speed, {center.x - size/2, center.y - size/2, center.z}},
		{extrudeLength, speed, {center.x + size/2, center.y - size/2, center.z}},
		{extrudeLength, speed, {center.x + size/2, center.y + size/2, center.z}},
		{extrudeLength, speed, {center.x - size/2, center.y + size/2, center.z}},
		{extrudeLength, speed, {center.x - size/2, center.y - size/2, center.z}}
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



static void showDemo()
{

	//printRectangle(10, {0, 0, 0});

	printLine({0,0,0}, {0,100, 0});

	/*
	for (int i = 0; i < 100; i += 5)
	{
		printRectangle(2*i, {0,0,0});
		//printLine({-150, i, 0}, {150, i, 0});
		//printCircle(i, { 0, 0, 0});
	}
	 */
	/*
	printCircle(10, {-100, 100, 0});
	printCircle(10, {-100, -100, 0});
	printCircle(10, {100, -100, 0});
	*/
}

void movementControl_showDemo(void)
{
	while(1)
	{
		showDemo();
	}
}

void movementControl_init()
{
	// Go to position {0, 0, 0}
	sendArmCommand({0, 0, 0}, 0);
}
