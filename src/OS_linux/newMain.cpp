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
			LOG(rxBuffer);

			lineComplete = false;
			rxIndex = 0;
		}

		readInput();
	}
//
//		bDirection = !bDirection;
//		int stepCount = 0;
//		for (int i = 0; i < 100; i++)
//		{
//			int stepCmd = (bDirection)? 1 : -1;
//			stepper1.OnUpdate(stepCmd, true);
//			stepper2.OnUpdate(stepCmd, true);
//			while((stepper1.getError() != 0 ) && (stepper2.getError() != 0))
//			{
//				stepper1.OnUpdateRegulation();
//				stepper2.OnUpdateRegulation();
//
//				stepper1.moveStart();
//				stepper2.moveStart();
//
//				stepper1.moveEnd();
//				stepper2.moveEnd();
//			}
	//	}
	//}

	int i = 0;
	while(1)
	{
		char rx = serial.read();
		LOG(i++);
		serial.write(rx);
	}

	thread_GUI.join();

	LOG("PROGRAM END");

	return 0;
}
