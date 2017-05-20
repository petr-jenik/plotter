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

    	return true;
    }

    return false;
}

#include "draw.h"


/*
 * Move along a line according to movement command
 *
 *@param[in] moveCommand cmd - start position, end position, defined speed, extruder state
 *@param[in] cmdBuffer - buffer for new armCommands
 */
void _createLine(const moveCommand& cmd, safe_queue<armCommand>& cmdBuffer)
{
	position startPos = cmd.pos1;
	position endPos = cmd.pos2;
	float speed = cmd.movementSpeed;
	bool extrude = cmd.extrude;

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

	// A close approximation to a straight line between two points
	for (float i = 0; i < distance; i = i + speed)
	{
		position currentPos;
		currentPos.x = startPos.x + dx*i;
		currentPos.y = startPos.y + dy*i;
		currentPos.z = startPos.z + dz*i;

		armCommand cmd;

		if (_setArmsToPosition(currentPos, cmd))
		{
			cmd.extrude = extrude;
			cmd.relPosZ = zAxeToRelative(currentPos.z);
			cmdBuffer.send(cmd);
		}
	}
}

void demo(safe_queue<armCommand> &queueOutput)
{
	float speed = 1;
	while(1)
	{
		position p1 = {-10, 0, 0};
		position p2 = {-10, 100, 0};
		position p3 = {10, 100, 0};
		position p4 = {10, 50, 0};

		//position p2 = {100, 0, 0};
		//position p3 = {100, 100, 0};
		//position p4 = {0, 100, 0};

		//_createLine(p1, p2, queueOutput, speed);
		//_createLine(p2, p1, queueOutput, speed);

		/*
		_createLine(p1, p2, queueOutput, speed);
		_createLine(p2, p3, queueOutput, speed);
		_createLine(p3, p4, queueOutput, speed);
		_createLine(p4, p1, queueOutput, speed);
		*/
		std::cout << "---------" << std::endl;
	}
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
		queueInput.receive(inputData);

		_createLine(inputData, queueOutput);

		//std::cout << "Thread: " << __FUNCTION__ << ", DATA: " << inputData << std::endl;
	}
}


