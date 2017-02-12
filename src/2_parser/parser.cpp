/*
 * parser.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include "app_threads.h"

using namespace std;

#include <iostream>
#include <fstream>

#include "nanosvg.h"

char * readFromFile(string fileName)
{
	streampos size;
	char * memblock = NULL;

	ifstream file (fileName, ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
	}
	return memblock;
}

NSVGimage* imageFromFile(string fileName)
{
    char * data = readFromFile(fileName);
	if (data == NULL)
	{
		cout << "Could not open SVG image.\n" << endl;
		return NULL;
	}

	//g_image = nsvgParseFromFile("nano.svg", "px", 96.0f);
	NSVGimage* g_image = nsvgParse(data, "px", 96.0f);
	if (g_image == NULL) {
		cout << "nsvgParse ERROR" << endl;
		return NULL;
	}
	delete data;
	return g_image;
}


safe_queue<guiCommand> * guiQueue = NULL;

void nsvg_sendCommand(guiCommand command)
//void sendCommand(guiCommand command)
{
	if (guiQueue != NULL)
	{
		guiQueue->send(command);
	}
}

void parser_loop(safe_queue<string> &queueInput, safe_queue<guiCommand> &queueOutput)
{
	guiQueue = &queueOutput;

	imageFromFile("1.svg");

	return;

	while(1)
	{
		imageFromFile("1.svg");

		string receivedData;
		queueInput.receive(receivedData);

		//std::cout << "Thread: " << __FUNCTION__ << ", DATA: " << receivedData <<std::endl;

		//if (receivedData == cKillSignal)
		//{
			//queueOutput.send(receivedData);
		//	break;
		//}
	}
}


