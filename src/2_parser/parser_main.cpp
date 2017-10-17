/*
 * parser.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include <cstdio>
#include <stdio.h>

#include "app_threads.h"
#include "parser.h"

//safe_queue<moveCommand> * guiQueue = NULL;

position topLeft;
position bottomRight;
bool bboxUsed = false;

void boundingBox(position start, position end, float movementSpeed, float extrude)
{
	//GuidrawLine(start, end);

	if (!bboxUsed)
	{
		topLeft = end;
		bottomRight = end;
		bboxUsed = true;
	}

	//if (extrude > 0)
	if (1)
	{
		topLeft.x = std::min(topLeft.x, end.x);
		topLeft.y = std::min(topLeft.y, end.y);

		bottomRight.x = std::max(bottomRight.x, end.x);
		bottomRight.y = std::max(bottomRight.y, end.y);
	}

	LOG("topLeft:" << topLeft);
	LOG("bottomRight:" << bottomRight);
}


void moveTo(position start, position end, float movementSpeed, float extrude)
{
	if (extrude > 0)
	{
		//LOG("Print from" << start << "to" << end << ", speed: " << movementSpeed);
	}
	else
	{
		LOG("Move from" << start << "to" << end << ", speed: " << movementSpeed << ", extrude:" << extrude);
	}

	LOG("extrude: " << extrude);

	moveCommand cmd;
	cmd.pos1 = start;
	cmd.pos2 = end;
	cmd.extrude = (extrude > 0) ? true: false;
	cmd.movementSpeed = movementSpeed;

	movementControl_createLine(cmd);

	//if (guiQueue != NULL)
	//{
	//	guiQueue->send(cmd);
	//}
}


void parser_loop(safe_queue<std::string> &queueInput)//, safe_queue<moveCommand> &queueOutput)
{
	//guiQueue = &queueOutput;

	//path_parser parser(10 , 4, -6);
	//path_parser parser(1 , 10, 10);
	path_parser parser(1);
	parser.registerMoveToCallback(moveTo);

	//parser.registerMoveToCallback(boundingBox);

	while(1)
	{
		std::string receivedData;
		queueInput.receive(receivedData);

		parser.newData(receivedData);
		//LOG("GCODE: " << receivedData);
		parser.update();

		//std::cout << "Thread: " << __FUNCTION__ << ", DATA: " << receivedData <<std::endl;

		//if (receivedData == cKillSignal)
		//{
			//queueOutput.send(receivedData);
		//	break;
		//}
	}
}


