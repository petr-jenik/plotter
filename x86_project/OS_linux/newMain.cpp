//============================================================================
// Name        : threads_example.cpp
// Author      : Apollo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <vector>

#include "reader_main.h"
#include "parser_main.h"
#include "movementControl_main.h"
#include "stepperControl_main.h"

#include "global.h"
#include "my_gui.h"
#include "Serial.h"


#include "project_config.h"
#include "hwStepperPins.h"
#include "servo.h"
#include "Timer.h"

#include "stepperControl_main.h"
#include "building_blocks.h"
#include <cstring> // memset

using namespace std;

void heartbeat(void)
{
	//LOG("Alive!");
}

void atLimitSwitch(int i)
{
	//("At limit switch. Channel: " << i);
}



void systemInit()
{
	// Order of initialisation is important!
	timeDelayInit();
	//stepperControl_init();
	//movementControl_init();
	//parser_init();
	//reader_init();
}

bool lineComplete = false;
char rxBuffer[512] = {0};
int rxIndex = 0;

Serial serial;

void readInput()
{
	char rx = serial.read();
	if (rx == ';')
	{
		lineComplete = true;
	}
	if (rxIndex < sizeof(rxBuffer))
	{
		rxBuffer[rxIndex++] = rx;
	}
}

const int cMaxNumberOfCmdParts = 10;

typedef struct
{
	char * parts[cMaxNumberOfCmdParts];
	int32_t length;
	bool used;
} command;


bool processInputData(char * pData, int dataLen, command * pCmdData)
{
	//char dataBuffer[512];
	command cmdData;
	//memset(dataBuffer, 0, sizeof(dataBuffer));
	memset(cmdData.parts, 0, sizeof(cmdData.parts));

	// Create a local copy of input data
	//size_t sizeToCopy = MIN(dataLen, sizeof(dataBuffer));
	//memcpy(dataBuffer, pData, sizeToCopy);

	const char delimiters[] = ",";
	char *token;

	/* get the first token */
	token = strtok(pData, delimiters);

	int i = 0;
	if (token != NULL)
	{
		cmdData.parts[i++] = token;
	}

	/* walk through other tokens */
	while( token != NULL )
	{
		token = strtok(NULL, delimiters);
		if (token != NULL)
		{
			cmdData.parts[i++] = token;
		}
	}

	cmdData.length = i;
	cmdData.used = (i) ? true : false;


	if (cmdData.length == 2)
	{
		*pCmdData = cmdData;
		return true;
	}
	else
	{
		LOG("Invalid input data: ");
		LOG(pData);
		return false;
	}
}

/*
int myAtoi(char * pData)
{
	char dataBuffer[512];
	memset(dataBuffer, 0, sizeof(dataBuffer));

	// Create a local copy of input data
	size_t sizeToCopy = MIN(strlen(pData), sizeof(dataBuffer));
	memcpy(dataBuffer, pData, sizeToCopy);

	LOG("data len:" << (uint32_t)strlen(pData));

	return atoi(dataBuffer);
}
*/

/*
 * List of tasks:
 *
 * 	1. reader - reads line of data from serial (or from file) - add data to parser
 * 	2. parser - parses data - create commands from them (move, line, etc)
 *  3. movement_control - reads commands and compute movements
 *  4. motor_control - controls steppers according to commands (get them to required position)
 *  5. (optional) GUI (plot actual state)
 *
 */
int main(int argc, char** argv)
{
	unsigned int pocetProcesoru = std::thread::hardware_concurrency();
	LOG("Pocet procesoru: " << pocetProcesoru);

	/* Create GUI loop before an initialization of the rest of the system */
	std::thread thread_GUI(gui_loop);

	/* App init */
	systemInit();

	Stepper stepper1;
	Stepper stepper2;

	stepper1.registerGPIOs(getStepperGPIOs(0));
	stepper2.registerGPIOs(getStepperGPIOs(1));

	while(1)
	{
		if (lineComplete)
		{
			command cmdData;

			if (processInputData(rxBuffer, rxIndex, &cmdData))
			{
				stepper1.forceMove(atoi(cmdData.parts[0]));
				stepper2.forceMove(atoi(cmdData.parts[1]));
			}

			// Clear input data
			memset(rxBuffer, 0, sizeof(rxBuffer));
			lineComplete = false;
			rxIndex = 0;
		}

		readInput();
	}

	thread_GUI.join();

	LOG("PROGRAM END");

	return 0;
}
