/*
 * merged mainApp.cpp and hwGpio.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

/*
 * gui.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include <plotterarm.h>
#include <plotterarmController.h>
#include <iostream>

#include "app_threads.h"
#include "draw.h"
#include "gui.h"
#include "math_tools.h"
#include "config.h"
#include "hwGpio.h"
#include "stepperSim.h"
#include "math_tools.h"
#include "global.h"
#include "stepperControl.h"
#include "math_tools.h"
#include "myGlobal.h"
#include "database.h"
#include "hwStepperPins.h"
#include "hwServo.h"
#include "servo.h"
#include "Timer.h"

using namespace std;

void servoInit()
{
	return;
}

bool gExtrude = false;

void servoSetPosition(bool turnOn, uint32_t channel)
{
	gExtrude = turnOn;
}

std::vector<moveCommand> drawList;
std::vector<moveCommand> drawListRequired;

std::mutex drawList_lock;
std::mutex drawListRequired_lock;

GpioDesc_t undefPin = {0, ePin_undef};
// ----------------------------------------------------------------------------

// HW simulation
StepperGui stepperGui1(pos_S1.x, pos_S1.y, armLength_AS1, left_stepper_offset);
StepperGui stepperGui2(pos_S2.x, pos_S2.y, armLength_BS2, right_stepper_offset);

StepperGui * steppers[] = {&stepperGui1, &stepperGui2};



bool _getEndpoint(position& C)
{
	position A = steppers[0]->getEndPoint();
	position B = steppers[1]->getEndPoint();
	position C1, C2;

	if (getIntersection(A, armLength_AC, B, armLength_BC, C1, C2))
	{
		float distC1 = getDistance(C1, pos_S1);
		float distC2 = getDistance(C2, pos_S2);

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

void gui_add_line(const moveCommand& cmd)
{
	std::lock_guard<std::mutex> hold(drawListRequired_lock);
	drawListRequired.push_back(cmd);
}



//TODO Move to class Gui and add some event handler, registerm all gui objects and redraw them at once
void drawAll(void)
{
    //Gui::clear();
    //Gui::glSelectColor(eColor_black);

    //stepperGui1.draw();
    //stepperGui2.draw();

    //return;
    //drawLine(stepperGui1.getEndPoint(), stepperGui2.getEndPoint());

    position C1, C2;
    //bool getIntersection(position A, float r1, position B, float r2, position &inter1, position &inter2);
    bool result = getIntersection(stepperGui1.getEndPoint(), armLength_AC, stepperGui2.getEndPoint(), armLength_BC, C1, C2);
    if (result == true)
    {
    	float dist1 = getDistance3D(pos_S1, C1);
    	float dist2 = getDistance3D(pos_S1, C2);

    	position C = (dist1 > dist2) ? C1 : C2;

        //Gui::glSelectColor(eColor_red);
        //drawLine(stepperGui1.getEndPoint(), C1);
        //drawLine(stepperGui2.getEndPoint(), C1);
        //Gui::glSelectColor(eColor_blue);
        ///drawLine(stepperGui1.getEndPoint(), C2);
        //drawLine(stepperGui2.getEndPoint(), C2);

       // Gui::glSelectColor(eColor_blue);
        //drawCircle(stepperGui1.getEndPoint(), 5);

        //Gui::glSelectColor(eColor_green);
        //drawCircle(stepperGui2.getEndPoint(), 5);

        //Gui::glSelectColor(eColor_black);

        static position currentPos = C;

		moveCommand outCmd;
		outCmd.extrude = gExtrude;
		outCmd.pos1 = currentPos;
		outCmd.pos2 = C;

#ifndef NO_GUI
		std::lock_guard<std::mutex> hold(drawList_lock);
		drawList.push_back(outCmd);
#else
		std::cout << "Head position: " << C << std::endl;
#endif //#ifndef NO_GUI
		currentPos = C;

		// Clear draw list for new layer
		//TODO Remove this 13.10.2017
		/*static float old_z = positionZ;
		if (positionZ != old_z)
		{
			drawList.clear();
		}*/
		//old_z = positionZ;

        //cout << "interception found" << endl;
        //cout << C1.x << ", " <<C.y << endl;
    }
}

Gpio::Gpio(GpioDesc_t gpioDesc)
{
    if (gpioDesc.pin == ePin_undef)
    {
        valid = false;
        return;
    }

    valid = true;
    this->devId = gpioDesc.devId;
    this->pin = gpioDesc.pin;
}

void
Gpio::powerUp ()
{
    if (false == valid)
    {
        return;
    }
#ifdef DEBUG_GPIO
    cout << "GPIO::powerUp()" << endl;
#endif
    // Start with led turned off
    turnOff ();
}

void
Gpio::turnOn ()
{
    if (false == valid)
    {
        return;
    }

    StepperGui * stepper = steppers[this->devId - 1];

    switch(this->pin)
    {
    case ePin_Dir:
        stepper->setDirection(true);
        break;

    case ePin_Step:
        stepper->move();
        drawAll();
        break;

    case ePin_Reset:
    case ePin_Sleep:
    case ePin_Enable:
    case ePin_LimSw1:
    case ePin_LimSw2:
    case ePin_undef:
        break;
    }

#ifdef DEBUG_GPIO
    cout << "GPIO::turnOn()" << endl;
#endif
}

void
Gpio::turnOff ()
{
    if (false == valid)
    {
        return;
    }

    switch(this->pin)
    {
    case ePin_Dir:
    {
        StepperGui * stepper = steppers[this->devId - 1];
    	stepper->setDirection(false);
        break;
    }
    default:
        break;
    }

#ifdef DEBUG_GPIO
    cout << "GPIO::turnOff()" << endl;
#endif
}

void Gpio::toggle ()
{
    if (false == valid)
    {
        return;
    }

#ifdef DEBUG_GPIO
    cout << "GPIO::toggle()" << endl;
#endif
}

bool Gpio::isOn ()
{
    if (false == valid)
    {
        return false;
    }

#ifdef DEBUG_GPIO
    cout << "GPIO::isOn()" << endl;
    cout << this->devId << "," << this->pin << endl;
#endif

    switch(pin)
    {
    case ePin_LimSw1:
    {
        StepperGui * stepper = steppers[this->devId - 1];
    	return stepper->isAtLimitPosition();
        break;
    }

    case ePin_LimSw2:
    {
        StepperGui * stepper = steppers[this->devId - 1];
        return stepper->isAtLimitPosition();
        break;
    }

    default:
        return false;
    }
}

bool Gpio::isValid()
{
#ifdef DEBUG_GPIO
    cout << "GPIO::isValid()" << endl;
#endif
    return this->valid;

}

//////////////////////////////////////////////////////////

void update(void)
{
    Gui::clear();

    ////color = static_cast<eColor>((static_cast<int>(color) + 1) % static_cast<int>(eColor_COUNT));
    //Gui::glSelectColor(color);

    if (1)
    {
    	std::lock_guard<std::mutex> hold(drawList_lock);
		for (auto command : drawList)
		{
			eColor color = (command.extrude)? eColor_blue : eColor_red;
			Gui::glSelectColor(color);
			drawLine((command.pos1), (command.pos2));
		}

    	std::lock_guard<std::mutex> hold2(drawListRequired_lock);
		for (auto command : drawListRequired)
		{
			eColor color = (command.extrude)? eColor_green : eColor_black;
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


void gui_loop(void)
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


/////////////////////////////////
