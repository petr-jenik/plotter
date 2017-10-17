/*
 * motorControl.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include "app_threads.h"

#include <cmath>

using namespace std;

void motorControl_loop(safe_queue<armCommand> &queueInput, safe_queue<stepperCommand> &queueOutput)
{
	while(1)
	{
		armCommand receivedData;
		queueInput.receive(receivedData);
		//while(!queueInput.receive(receivedData))
		//{
		//}

		LOG("Thread: " << __FUNCTION__ << ", DATA: [" << receivedData.relativeAngle1 << "," << receivedData.relativeAngle2 << "]");

		stepperCommand cmd;
		cmd.stepper1 = receivedData.relativeAngle1 * 1000;
		cmd.stepper2 = receivedData.relativeAngle2 * 1000;
		cmd.stepper3 = 0;
		cmd.extrude = receivedData.extrude;
		queueOutput.send(cmd);
	}
}
