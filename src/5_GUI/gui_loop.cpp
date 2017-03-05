/*
 * gui.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include "app_threads.h"

#include "draw.h"
#include "gui.h"

#include "stepper_sim.h"
#include "math_tools.h"
#include "config.h"

using namespace std;

std::vector<moveCommand> drawList;
std::mutex drawList_lock;


// HW simulation
StepperGui stepperGui1(pos_S1.x, pos_S1.y, armLength_AS1, 90);
StepperGui stepperGui2(pos_S2.x, pos_S2.y, armLength_BS2, 90);

StepperGui * steppers[] = {&stepperGui1, &stepperGui2};

bool _getEndpoint(position& C)
{
	position A = steppers[0]->getEndPoint();
	position B = steppers[1]->getEndPoint();
	position C1, C2;

	if (getInterception(A, armLength_AC, B, armLength_BC, C1, C2))
	{
		float distC1 = getDistance(C1, pos_S1);
		float distC2 = getDistance(C2, pos_S2);

		C = (distC1 < distC2)? C2 : C1;
		return true;
	}
	else
	{
		std::cout << A << B << C1 << C2;
		return false;
	}
}


void drawSteppers()
{
	// Draw stepper
	Gui::glSelectColor(eColor_red);

	for (int i = 0; i < sizeof(steppers)/sizeof(steppers[0]); i++)
	{
		StepperGui * stepper = steppers[i];
		stepper->draw();
	}

	position C;//1, C2;
	position A = steppers[0]->getEndPoint();
	position B = steppers[1]->getEndPoint();

	Gui::glSelectColor(eColor_red);

	if(_getEndpoint(C))
	{
		drawLine(A, C);
		//drawLine(A,C2);
		drawLine(B, C);
		//drawLine(B,C2);
	}
}

void mouseHandler(int button, int state, int x, int y)
{
    cout << "Buton: " << button << endl;
    cout << "State: " << state << endl;
    cout << "x: " << x << endl;
    cout << "y: " << y << endl;
}

void update(void)
{
    Gui::clear();

    ////color = static_cast<eColor>((static_cast<int>(color) + 1) % static_cast<int>(eColor_COUNT));
    //Gui::glSelectColor(color);

    {
    	std::lock_guard<std::mutex> hold(drawList_lock);
		for (auto command : drawList)
		{
			eColor color = (command.type == eLine)? eColor_blue : eColor_red;
			Gui::glSelectColor(color);
			drawLine((command.pos1), (command.pos2));
		}
    }
    drawSteppers();

    // Remove this
	position p1 = {-100, 0, 0};
	position p2 = {100, 0, 0};

	position p3 = {0, -50, 0};
	position p4 = {0, 50, 0};

	drawLine(p1, p2);

    drawLine(p3, p4);
    //
}

void _receiveFromQueue(safe_queue<armCommand> &queueInput)
{
	while(1)
	{
		armCommand command;
		queueInput.receive(command);

		static float zPosition = command.zPosition;

		/*
		if (zPosition != command.zPosition)
		{
	    	std::lock_guard<std::mutex> hold(drawList_lock);
			drawList.erase(drawList.begin(),drawList.end());
		}
		*/

		float angle1 = relativeToAngle(command.relativeAngle1);
		float angle2 = relativeToAngle(command.relativeAngle2);

		//LOG("GUI_loop: Angles: " << angle1 << "," << angle2);

		steppers[0]->setAngle(angle1);
    	steppers[1]->setAngle(angle2);
    	steppers[0]->update();
    	steppers[1]->update();

    	zPosition = command.zPosition;

		position C;//, C2;

		if (_getEndpoint(C))
		{
			static position currentPos = C;

			moveCommand cmd;
			cmd.type = eLine;
			cmd.pos1 = currentPos;
			cmd.pos2 = C;

			currentPos = C;

			//LOG("Line:" << cmd);
			{
				std::lock_guard<std::mutex> hold(drawList_lock);
				drawList.push_back(cmd);
			}
		}

        //std::cout << "Thread: " << __FUNCTION__ << ", DATA: ";

		/*cout << "type: " << command.type;
		cout << ", x1" << command.pos1.x;
		cout << ", y1" << command.pos1.y;
		cout << ", x2" << command.pos2.x;
		cout << ", y2" << command.pos2.y << endl;
		*/
	}
}

void GUI_loop(safe_queue<armCommand> &queueInput)
{
	cout << "Plotter simulation" << endl;

	Gui::guiInit(0, NULL);
    Gui::registerUpdateCallback(update);
    Gui::registerMouseCallback(mouseHandler);

    std::thread thr_receive(&_receiveFromQueue, std::ref(queueInput));

	Gui::guiMainLoop();

	cout << "GUI closed" << endl;

	thr_receive.join();
}
