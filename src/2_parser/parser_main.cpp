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


path_parser gParser(1);


// Forward declarartion
void moveTo(position end, float movementSpeed, float extrude);

void parser_init(void)
{
	gParser.registerMoveToCallback(moveTo);
}

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


void moveTo(position finalPosition, float movementSpeed, float extrudeLength)
{
	moveCommand cmd;
	cmd.finalPosition = finalPosition;
	cmd.extrudeLength = extrudeLength;
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

	//parser.registerMoveToCallback(boundingBox);

	while(1)
	{
		std::string receivedData;
		queueInput.receive(receivedData);

		gParser.newData(receivedData);
		LOG("GCODE: " << receivedData);
		gParser.update();

		//std::cout << "Thread: " << __FUNCTION__ << ", DATA: " << receivedData <<std::endl;

		//if (receivedData == cKillSignal)
		//{
			//queueOutput.send(receivedData);
		//	break;
		//}
	}
}


void parser_update(std::string receivedData)
{
		gParser.newData(receivedData);
		LOG("GCODE: " << receivedData);
		gParser.update();
}

