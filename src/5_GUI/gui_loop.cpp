/*
 * gui.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include "app_threads.h"

#include "draw.h"
#include "gui.h"

using namespace std;

std::vector<guiCommand> drawList;

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

    static eColor color = eColor_blue;
    color = static_cast<eColor>((static_cast<int>(color) + 1) % static_cast<int>(eColor_COUNT));
    Gui::glSelectColor(color);

    for (auto command : drawList)
    {
    	drawLine(command.pos1,command.pos2);
    }
}

void _receiveFromQueue(safe_queue<guiCommand> &queueInput)
{
	while(1)
	{
	    guiCommand command;
		queueInput.receive(command);

		drawList.push_back(command);

        std::cout << "Thread: " << __FUNCTION__ << ", DATA: ";

		cout << "type: " << command.type;
		cout << ", x1" << command.pos1.x;
		cout << ", y1" << command.pos1.y;
		cout << ", x2" << command.pos2.x;
		cout << ", y2" << command.pos2.y << endl;
	}
}

void GUI_loop(safe_queue<guiCommand> &queueInput)
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
