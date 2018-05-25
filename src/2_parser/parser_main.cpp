/*
 * parser.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include "parser_main.h"
#include "movementControl_main.h"
#include "parser.h"


path_parser gParser(0.5);


// Forward declarartion
void moveTo(position end, float movementSpeed, float extrude);

void parser_init(void)
{
	TRACE; // Trace macro
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
}

void parser_update(const char* pData, const unsigned int dataLength)
{
		gParser.newData(pData, dataLength);
		//LOG("GCODE: " << receivedData);
		gParser.update();
}

