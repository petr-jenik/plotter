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

bool fce(position C, armCommand& output)
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

    result = result and getInterception(C, armLength_AC, pos_S1, armLength_AS1, A1, A2);
    result = result and getInterception(C, armLength_BC, pos_S2, armLength_BS2, B1, B2);

    //std::cout << "Interception of" << C << "and " << pos_S1 << pos_S2 << std::endl;
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

    	output.relativeAngle1 = angleToRelative(angle1);
    	output.relativeAngle2 = angleToRelative(angle2);
    	output.zPosition = 0;
    	return true;
    }

    return false;
}

#include "draw.h"

void _createLine(position startPos, position endPos, safe_queue<armCommand>& cmdBuffer, float speed)
{
	float distance = getDistance(startPos, endPos);

	float dx = (endPos.x - startPos.x) / distance;
	float dy = (endPos.y - startPos.y) / distance;

	// TODO Velky problem - hodnotafloat  X a y prestreli o modulo(speed)

	for (float i = 0; i < distance; i = i + speed)
	{
		position currentPos;
		currentPos.x = startPos.x + dx*i;
		currentPos.y = startPos.y + dy*i;

		armCommand cmd;

		if (fce(currentPos, cmd))
		{
			cmdBuffer.send(cmd);
		}

		//auto delay = std::chrono::milliseconds(10);
		//std::this_thread::sleep_for(delay);
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

		_createLine(p1, p2, queueOutput, speed);
		_createLine(p2, p3, queueOutput, speed);
		_createLine(p3, p4, queueOutput, speed);
		_createLine(p4, p1, queueOutput, speed);
		std::cout << "---------" << std::endl;
	}
}


void movementControl_loop(safe_queue<moveCommand> &queueInput, safe_queue<armCommand> &queueOutput)
{
	while(1)
	{
		moveCommand inputData;
		queueInput.receive(inputData);

		//drawLine(inputData.pos1, inputData.pos2);

		//Start position
		position startPos = inputData.pos1;

		//End position
		position endPos = inputData.pos2;

		//Type of movement
		switch(inputData.type)
		{
			case eMove:
				break;

			case eLine:
				break;

			default:
				break;

		}

		float speed = 1;

		_createLine(startPos, endPos, queueOutput, speed);

		std::cout << "Thread: " << __FUNCTION__ << ", DATA: " << inputData << std::endl;

		//if (receivedData == cKillSignal)
		//{
			//queueOutput.send(receivedData);
		//	break;
		//}

		//std::ostringstream os;
		//os << "requested_steppers_positions(" << receivedData << ")"

	}
}


