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


// TODO Barevne oddelit jestli se jedna o rameno AS1 nebo BS2

// HW simulation

position gui_S1 = {100, 0, 0};
position gui_S2 = {100, 0, 0};

StepperGui stepperGui1(gui_S1.x, gui_S1.y, armLength_AS1, 90);
StepperGui stepperGui2(gui_S2.x, gui_S2.y, armLength_BS2, 90);

StepperGui * steppers[] = {&stepperGui1, &stepperGui2};

bool _getEndpoint(position& C)
{
	position A = steppers[0]->getEndPoint();
	position B = steppers[1]->getEndPoint();
	position C1, C2;

	if (getIntersection(A, armLength_AC, B, armLength_BC, C1, C2))
	{
		float distC1 = getDistance(C1, gui_S1);
		float distC2 = getDistance(C2, gui_S2);

		C = (distC1 < distC2)? C2 : C1;
		return true;
	}
	else
	{
		//std::cout << A << B << C1 << C2;
		return false;
	}
}


void drawSteppers()
{
	// Draw stepper
	Gui::glSelectColor(eColor_green);
	steppers[0]->draw();

	Gui::glSelectColor(eColor_blue);
	steppers[1]->draw();

	Gui::glSelectColor(eColor_black);

	position C;//1, C2;
	position A = steppers[0]->getEndPoint();
	position B = steppers[1]->getEndPoint();

	Gui::glSelectColor(eColor_black);

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
			eColor color = (command.extrude)? eColor_blue : eColor_red;
			Gui::glSelectColor(color);
			drawLine((command.pos1), (command.pos2));
		}
    }
    drawSteppers();

    // Remove this - X and Y axis
	position p1 = {-100, 0, 0};
	position p2 = {100, 0, 0};

	position p3 = {0, -50, 0};
	position p4 = {0, 50, 0};

	drawLine(p1, p2);
    drawLine(p3, p4);
    //
}

//void _receiveFromQueue(safe_queue<armCommand> &queueInput)
//{
	//while(1)
	//{
void gui_parseCommand(armCommand inputCmd)
{
		//armCommand inputCmd;
		//queueInput.receive(inputCmd);

		static float positionZ = relativeToZAxe(inputCmd.relPosZ);

		/*
		if (zPosition != command.zPosition)
		{
	    	std::lock_guard<std::mutex> hold(drawList_lock);
			drawList.erase(drawList.begin(),drawList.end());
		}
		*/

		float angle1 = relativeToAngle(inputCmd.relativeAngle1);
		float angle2 = relativeToAngle(inputCmd.relativeAngle2);

		//LOG("GUI_loop: Angles: " << angle1 << "," << angle2);

		steppers[0]->setAngle(angle1);
    	steppers[1]->setAngle(angle2);
    	steppers[0]->update();
    	steppers[1]->update();


    	positionZ = relativeToZAxe(inputCmd.relPosZ);

		position C;//, C2;

		if (_getEndpoint(C))
		{
			static position currentPos = C;

			moveCommand outCmd;
			outCmd.extrude = inputCmd.extrude;
			outCmd.pos1 = currentPos;
			outCmd.pos2 = C;

			currentPos = C;

			// Clear draw list for new layer
			//TODO Remove this 13.10.2017
			static float old_z = positionZ;
			if (positionZ != old_z)
			{
				drawList.clear();
			}
			old_z = positionZ;


			if (inputCmd.extrude == true)
			//LOG("Line:" << outCmd);
			{
				std::lock_guard<std::mutex> hold(drawList_lock);
				drawList.push_back(outCmd);
			}
		}

        //std::cout << "Thread: " << __FUNCTION__ << ", DATA: ";

		/*cout << "type: " << command.type;
		cout << ", x1" << command.pos1.x;
		cout << ", y1" << command.pos1.y;
		cout << ", x2" << command.pos2.x;
		cout << ", y2" << command.pos2.y << endl;
		*/
	//}
}

void GUI_loop(safe_queue<armCommand> &queueInput)
{
	cout << "Plotter simulation" << endl;

	Gui::guiInit(0, NULL);
    Gui::registerUpdateCallback(update);
    Gui::registerMouseCallback(mouseHandler);

    //std::thread thr_receive(&_receiveFromQueue, std::ref(queueInput));

	Gui::guiMainLoop();

	cout << "GUI closed" << endl;

	//thr_receive.join();
}
