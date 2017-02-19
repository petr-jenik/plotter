/*
 * parser.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include <cstdio>
#include <stdio.h>

#include "parser.h"
#include "app_threads.h"

std::ostream& operator<< (std::ostream& stream, position pos)
{
	stream << "[" << pos.x << "," << pos.y << "," << pos.z << "]";
	return stream;
}

safe_queue<guiCommand> * guiQueue = NULL;

void moveTo(position start, position end, float movementSpeed, float extrude)
{
	if (extrude)
	{
		std::cout << "Print from";
	}
	else
	{
		std::cout << "Move from";
	}

	std::cout << start << "to" << end << ", speed: " << movementSpeed << std::endl;

	guiCommand cmd;
	cmd.pos1 = start;
	cmd.pos2 = end;
	cmd.type = (extrude) ? eLine : eMove;

	if (guiQueue != NULL)
	{
		guiQueue->send(cmd);
	}
}


void parser_loop(safe_queue<std::string> &queueInput, safe_queue<guiCommand> &queueOutput)
{
	guiQueue = &queueOutput;

	path_parser parser(1);
	parser.registerMoveToCallback(moveTo);

	while(1)
	{
		std::string receivedData;
		queueInput.receive(receivedData);

		parser.newData(receivedData);
		parser.update();

		//std::cout << "Thread: " << __FUNCTION__ << ", DATA: " << receivedData <<std::endl;

		//if (receivedData == cKillSignal)
		//{
			//queueOutput.send(receivedData);
		//	break;
		//}
	}
}


