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

using namespace std;


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
bool _setArmsToPosition(position C, armCommand& outputCmd)
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

    	float angle1 = getAngle(A, pos_S1);
    	float angle2 = getAngle(B, pos_S2);

		//LOG("movementControl_loop: Angles: " << angle1 << "," << angle2);
    	//LOG("ANGLE1: " << angle1);
    	//LOG("ANGLE2: " << angle2);
    	//std::cout << "ANGLE1: " << angle1 << std::endl;
    	//std::cout << "ANGLE2: " << angle2 << std::endl;

    	// Fill only arm positions
    	// Extruder status and relative position in Z axe will be filled in different place
    	outputCmd.relativeAngle1 = angleToRelative(angle1);
    	outputCmd.relativeAngle2 = angleToRelative(angle2);
        outputCmd.relPosZ = zAxeToRelative(C.z);

    	return true;
    }

    return false;
}

#include "draw.h"

void sendArmCommand(position pos, bool extrude)
{
	armCommand armCmd;
	if (_setArmsToPosition(pos, armCmd))
	{
		armCmd.extrude = extrude;
		gui_parseCommand(armCmd);
		//cmdBuffer.send(armCmd);
	}
}


/*
 * Move along a line according to movement command
 *
 *@param[in] moveCommand cmd - start position, end position, defined speed, extruder state
 *@param[in] cmdBuffer - buffer for new armCommands
 */
//void _createLine(const moveCommand& cmd, safe_queue<armCommand>& cmdBuffer)
void movementControl_createLine(const moveCommand& cmd)
{
	position startPos = cmd.pos1;
	position endPos = cmd.pos2;
	float speed = cmd.movementSpeed;
	bool extrude = cmd.extrude;


	// get distance between start and end points
	float distance = getDistance3D(startPos, endPos);

	if (distance == 0)
	{
		return;
	}

	float dx = (endPos.x - startPos.x) / distance;
	float dy = (endPos.y - startPos.y) / distance;
	float dz = (endPos.z - startPos.z) / distance;

	// Run next loop at least once
	//distance = max(distance, speed);

	//TODO Fix this horrible solution - this should add at least 2 point from each row
	float step = min(speed, distance / 2);

	if (-0.001 < speed && speed < 0.0001)
	{
		speed = 1;
	}
	// A close approximation to a straight line between two points

	// TODO 13.10.2017 - This loop causes all the troubles!!!!!
	/*for (float i = 0; i < distance; i = i + speed)
	{
		position currentPos;
		currentPos.x = startPos.x + dx*i;
		currentPos.y = startPos.y + dy*i;
		currentPos.z = startPos.z + dz*i;

	}
	*/
	sendArmCommand(startPos, extrude);
	sendArmCommand(endPos, extrude);
}

const float ds = 50; // demo size

position p1 = {0, 0, 0};
position p2 = {0, ds, 0};
position p3 = {ds, ds, 0};
position p4 = {ds, 0, 0};

position l1 = {-100, 0, 0};
position l2 = {100,  0, 0};
position l3 = {0, 100, 0};
position l4 = {0, -100, 0};

moveCommand demoCommands[] =
{
	{true, 0.00001, l1, l2},
	{true, 0.1, l2, l1},
	//{true, 0.001, l3, l4},
	//{true, 0.001, l4, l3}
	//{true, 0.01, l1, l2},
	//{true, 0.01, l2, l1}
	/*{true, 0.01, p1, p2},
	{true, 0.01, p2, p3}
	{true, 0.01, p3, p4},
	{true, 0.01, p4, p1},
	{true, 0.01, p1, p3},
	{true, 0.01, p3, p1},
	{true, 0.01, p1, p2},
	{false, 0.01, p2, p4},
	{false, 0.01, p4, p2},
	{true, 0.01, p2, p1}*/
};

const int32_t demoCmdsCount = (sizeof(demoCommands)/sizeof(demoCommands[0]));

void demoReceive(moveCommand &cmd)
{
	static int32_t index = 0;

	cmd = demoCommands[index];
	index = (index + 1) % demoCmdsCount;
}

/*
 * This task(loop) controls movement of arms
 *
 * Allows only movement along the straight lines
 */

void movementControl_loop(safe_queue<moveCommand> &queueInput, safe_queue<armCommand> &queueOutput)
{
	while(1)
	{
		moveCommand inputData;

		// Movement test
		queueInput.receive(inputData);
		//demoReceive(inputData);

		movementControl_createLine(inputData);

		//std::cout << "Thread: " << __FUNCTION__ << ", DATA: " << inputData << std::endl;
	}
}


