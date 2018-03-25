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

//#include <armController.h>
//#include <plotterArm.h>
#include <iostream>
#include <mutex>

#include "stepperControl_main.h"
#include "reader_main.h"
#include "parser_main.h"
#include "movementControl_main.h"
#include "stepperControl_main.h"

#include "draw.h"
#include "gui.h"
#include "math_tools.h"
#include "config.h"
#include "hwGpio.h"
#include "stepperSim.h"
#include "math_tools.h"
#include "global.h"
#include "math_tools.h"
#include "hwStepperPins.h"
#include "hwServo.h"
#include "servo.h"
//#include "Timer.h"

using namespace std;

void servoInit()
{
	return;
}

bool gExtrude = false;

void servoSetPosition(float angle, uint32_t channel)
{
	gExtrude = (angle < 90)? false : true;
}

struct guiCommandColor
{
	guiCommand cmd;
	eColor color;
};

std::vector<guiCommand> drawList;
std::vector<struct guiCommandColor> drawListRequired;

std::mutex drawList_lock;
std::mutex drawListRequired_lock;

GpioDesc_t undefPin = {0, ePin_undef};
// ----------------------------------------------------------------------------

// HW simulation
StepperGui stepperGui1(pos_S1.x, pos_S1.y, armLength_AS1, LEFT_ARM_OFFSET);
StepperGui stepperGui2(pos_S2.x, pos_S2.y, armLength_BS2, RIGHT_ARM_OFFSET);

StepperGui * steppers[] = {&stepperGui1, &stepperGui2};

StepperGPIOs* pStepperGPIOs1 = NULL;
StepperGPIOs* pStepperGPIOs2 = NULL;

typedef struct {
	ePinType pinType;
	int stepperIndex;
} UsedPinDesc_t;

UsedPinDesc_t getUsedPinDesc(Gpio* pObject)
{
	if (pObject == NULL)
	{
		return {ePin_undef, -1};
	}

	StepperGPIOs* pStepperGPIOs = NULL;
	int idx = 0;
	while((pStepperGPIOs = getStepperGPIOs(idx)) != NULL)
	{
		if (*pObject == pStepperGPIOs->directionPin) { return {ePin_Dir,    idx};}
		if (*pObject == pStepperGPIOs->enablePin)    { return {ePin_Enable, idx};}
		if (*pObject == pStepperGPIOs->resetPin)     { return {ePin_Reset,  idx};}
		if (*pObject == pStepperGPIOs->sleepPin)     { return {ePin_Sleep,  idx};}
		if (*pObject == pStepperGPIOs->stepPin)      { return {ePin_Step,   idx};}
		idx++;
	}

	LimitSwitchGPIOs* pLimitSwitchGPIOs = NULL;
	idx = 0;
	while((pLimitSwitchGPIOs = getLimitSwitchGPIOs(idx)) != NULL)
	{
		if (*pObject == pLimitSwitchGPIOs->switchPin1) { return {ePin_LimSw1, idx};}
		if (*pObject == pLimitSwitchGPIOs->switchPin2) { return {ePin_LimSw2, idx};}
		idx++;
	}

	return {ePin_undef, -1};
}

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
	else
	{
		LOG("Invalid endpoint: " << C);
	}
}


void mouseHandler(int button, int state, int x, int y)
{
    cout << "Buton: " << button << endl;
    cout << "State: " << state << endl;
    cout << "x: " << x << endl;
    cout << "y: " << y << endl;
    if((button == 0) && (state == 1))
    {
        position clickPosition = {x, y, 0};
        setCenterPoint(clickPosition);
	}
}


void gui_add_line(const guiCommand& cmd, eColor color)
{
	std::lock_guard<std::mutex> hold(drawListRequired_lock);
	drawListRequired.push_back({cmd, color});
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

    	static position tmpC = C;
    	static position currentPos = C;

    	float k = 1;

    	tmpC.x = k * C.x + (1 - k) * tmpC.x;
    	tmpC.y = k * C.y + (1 - k) * tmpC.y;
    	tmpC.z = k * C.z + (1 - k) * tmpC.z;


		guiCommand outCmd;
		outCmd.extrudeLength = gExtrude;
		outCmd.startPosition = currentPos;
		outCmd.endPosition = tmpC;

#ifndef NO_GUI
		std::lock_guard<std::mutex> hold(drawList_lock);
		drawList.push_back(outCmd);
#else
		std::cout << "Head position: " << C << std::endl;
#endif //#ifndef NO_GUI
		currentPos = tmpC;

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


#define PIN_MASK(_N)    (1 << (_N))


bool Gpio::operator==(Gpio &A)
{
	if ((A.fBitMask == this->fBitMask) &&
		(A.fBitNumber == this->fBitNumber) &&
		(A.fIsActiveLow == this->fIsActiveLow) &&
		(A.fPortNumber == this->fPortNumber) &&
		(A.mode == this->mode) &&
		(A.pull == this->pull)&&
		(A.valid == this->valid))
	{
		return true;
	}
	else
	{
		return false;
	}
}



Gpio::Gpio(GpioDesc_t gpioDesc)
{
    if ((gpioDesc.pin == gpioPinUndef) || (gpioDesc.port == gpioPortUndef))
    {
        valid = false;
        return;
    }

    valid = true;
    pull = gpioDesc.pull;
    mode = gpioDesc.mode;
    fPortNumber = gpioDesc.port;
    fBitNumber = gpioDesc.pin;
    fIsActiveLow = gpioDesc.activeLow;
    fBitMask = PIN_MASK(fBitNumber);
}

void Gpio::powerUp ()
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

    UsedPinDesc_t pinDesc = getUsedPinDesc(this);
    ePinType pin = pinDesc.pinType;
    int devId = pinDesc.stepperIndex;

    switch(pin)
    {
    case ePin_Dir:
    {
        StepperGui * stepper = steppers[devId];
    	stepper->setDirection(true);
        break;
    }
    case ePin_Step:
    {
    	StepperGui * stepper = steppers[devId];
    	stepper->move();
        drawAll();
        break;
    }
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

    UsedPinDesc_t pinDesc = getUsedPinDesc(this);
    ePinType pin = pinDesc.pinType;
    int devId = pinDesc.stepperIndex;

    switch(pin)
    {
    case ePin_Dir:
    {
        StepperGui * stepper = steppers[devId];
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

    UsedPinDesc_t pinDesc = getUsedPinDesc(this);
    ePinType pin = pinDesc.pinType;
    int devId = pinDesc.stepperIndex;

#ifdef DEBUG_GPIO
    cout << "GPIO::isOn()" << endl;
    cout << this->devId << "," << this->pin << endl;
#endif

    switch(pin)
    {
    case ePin_LimSw1:
    {
        StepperGui * stepper = steppers[devId];
    	return stepper->isAtLimitPosition();
        break;
    }

    case ePin_LimSw2:
    {
        StepperGui * stepper = steppers[devId];
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
    	Gui::drawLinesStart();
    	std::lock_guard<std::mutex> hold(drawList_lock);
		for (auto command : drawList)
		{
			eColor color = (command.extrudeLength > 0)? eColor_blue : eColor_red;
			Gui::glSelectColor(color);
			//Gui::glSelectColor(eColor_green);
			//if (command.extrudeLength > 0)
			//{
				Gui::drawLines(center(command.startPosition));
				//Gui::glSelectColor(eColor_red);
				Gui::drawLines(center(command.endPosition));
				//drawLine((command.startPosition), (command.endPosition));
			//}
		}
    	Gui::drawLinesEnd();


    	Gui::drawLinesStart();
    	std::lock_guard<std::mutex> hold2(drawListRequired_lock);
		for (auto item : drawListRequired)
		{
			eColor color = (item.cmd.extrudeLength)? item.color : eColor_black;
			Gui::glSelectColor(color);
			Gui::drawLines(center(item.cmd.startPosition));
			Gui::drawLines(center(item.cmd.endPosition));
			//drawLine((item.cmd.startPosition), (item.cmd.endPosition));
		}
    	Gui::drawLinesEnd();
    }
    drawSteppers();

    // Remove this - X and Y axis
	position p1 = {-100, 0, 0};
	position p2 = {100, 0, 0};

	position p3 = {0, -50, 0};
	position p4 = {0, 50, 0};

	drawLine(p1, p2);
    drawLine(p3, p4);


    Gui::flush();
}

void keyboardHandler(unsigned char x, int y, int z)
{
	switch (x)
	{
	case '+':
		increaseZoom();
		break;

	case '-':
		decreaseZoom();
		break;

	default:
		break;

	}
	cout << "x: " << x << ", y: " << y << ", z: "<< z<< endl;
}

static void myTimerEvent(int te)
{
	//update player and other objects pos.
	Gui::registerTimerFunction(10, myTimerEvent, 1);
	//glutTimerfunc( 10, My_timer_event, 1); //Timer is a one shot have to restart it!!! important you have this....
	Gui::forceRedraw();
}

void gui_loop(void)
{
	cout << "Plotter simulation" << endl;

	Gui::guiInit(0, NULL);

    //Gui::registerIdleFunction(update);
    Gui::registerDisplayFunction(update);
    Gui::registerTimerFunction(10, myTimerEvent, 1);

    //Gui::registerUpdateCallback(update);
    Gui::registerMouseCallback(mouseHandler);
    Gui::registerKeyboardCallback(keyboardHandler);

	Gui::guiMainLoop();

	cout << "GUI closed" << endl;
}


/////////////////////////////////
