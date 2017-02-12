/*
 * motorControl.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include "app_threads.h"

using namespace std;

void motorControl_loop(safe_queue<string> &queueInput, safe_queue<string> &queueOutput)
{
	while(1)
	{
		string receivedData;
		queueInput.receive(receivedData);
		//while(!queueInput.receive(receivedData))
		//{
		//}

		std::cout << "Thread: " << __FUNCTION__ << ", DATA: " << receivedData <<std::endl;

		if (receivedData == cKillSignal)
		{
			queueOutput.send(receivedData);
			break;
		}

		std::ostringstream os;
		os << "stepper_update_loop(" << receivedData << ")";

		queueOutput.send(os.str());

	}
}
