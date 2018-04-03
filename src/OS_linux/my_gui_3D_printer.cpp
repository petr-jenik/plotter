/*
 * my_gui_3D_printer.cpp
 *
 *  Created on: 26 Mar 2018
 *      Author: pi
 */
#include "project_config.h"
#if PRINTER_TYPE == PRINTER_TYPE_3D_PRINTER
#include <mutex>
#include <vector>

#include "draw.h"
#include "gui.h"
#include "math_tools.h"
#include "hwGpio.h"
#include "math_tools.h"
#include "global.h"
#include "hwStepperPins.h"
#include "hwServo.h"

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
// Servo stubs
void hwServoInit(int32_t channel){};
void hwServoSetPosition(float angle, uint32_t channel){};

// --------------------------------

class LimStepperGui
{
    position startPos;
    position endPos;
    int stepCount; //Number of steps
    bool directionLeft;

	position currentPosition;

    float stepsPerOneTurn; //Number of steps for one turn of the stepper motor
    float pitching;// Stoupani - Distance traveled by the screed per one revolution
    float length;
public:
    LimStepperGui(position _start, position _end, float _pitching = 0.1, float _stepsPerOneTurn = 200)
	: startPos(_start),
	  endPos(_end),
	  stepCount(0),
	  pitching(_pitching),
	  stepsPerOneTurn(_stepsPerOneTurn)
{
       length = getDistance3D(startPos, endPos);
       directionLeft = true;
       this->update();
    };

    void shift(position relativeMove)
    {
    	this->startPos = relativeMove + this->startPos;
    	this->endPos   = relativeMove + this->endPos;
    }

    void draw(void)
    {
    	drawLine(startPos, endPos);
    	drawCircle(this->getCurrentPosition(), 2);
    }

    position getCurrentPosition(void)
    {
    	this->update();
    	return currentPosition;
    }

    void setDirection(bool directionLeft)
    {
        this->directionLeft = directionLeft;
    }

    bool getDirection(void)
    {
        return this->directionLeft;
    }

    void move(void)
    {
        if (getDirection() == true)
        {
            //Decrease position
            this->stepCount -= STEP_SIZE;
        }
        else
        {
            //Increase position
            this->stepCount += STEP_SIZE;
        }
        this->update();

    }

    bool isAtLimitPosition(void)
    {
		float maxNumberOfRevolutions = length / pitching;
    	int maxNumberOfSteps = stepsPerOneTurn * maxNumberOfRevolutions;
    	return (isInRange(stepCount, 0, maxNumberOfSteps) == false);
    }

    void update()
    {
		float maxNumberOfRevolutions = length / pitching;
    	int maxNumberOfSteps = stepsPerOneTurn * maxNumberOfRevolutions;

    	float relativeDistance = templateMap((float)stepCount, (float)0, (float)maxNumberOfSteps, 0.0f, 1.0f);

    	position diff = endPos;
    	diff = diff - startPos;
    	currentPosition.x = startPos.x + (diff.x * relativeDistance);
    	currentPosition.y = startPos.y + (diff.y * relativeDistance);
    	currentPosition.z = startPos.z + (diff.z * relativeDistance);
    }
};


// HW simulation
LimStepperGui limStepperGuiX({pos_START.x, 0, 0}, {pos_END.x, 0, 0}, 1);
LimStepperGui limStepperGuiY({0, pos_START.y, 0}, {0, pos_END.y, 0}, 1);
LimStepperGui limStepperGuiZ({0, 0, pos_START.z}, {0, 0, pos_END.z}, 1);

LimStepperGui * limSteppers[] = {&limStepperGuiX, &limStepperGuiY, &limStepperGuiZ};

/*
StepperGPIOs* pStepperGPIOs1 = NULL;
StepperGPIOs* pStepperGPIOs2 = NULL;
StepperGPIOs* pStepperGPIOs3 = NULL;
*/

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
	position X = limSteppers[eStepperIdx_X]->getCurrentPosition();
	position Y = limSteppers[eStepperIdx_Y]->getCurrentPosition();
	position Z = limSteppers[eStepperIdx_Z]->getCurrentPosition();

	// TODO fix this horrible sollution - there could be an angle offset (rotation of the whole axis)
	C.x = X.x;
	C.y = Y.y;
	C.z = Z.z;
	return true;
}


void drawSteppers()
{
	// Draw stepper
	Gui::glSelectColor(eColor_green);
	limSteppers[eStepperIdx_X]->draw();

	// Move Y stepper in X axis
	position posX = limSteppers[eStepperIdx_X]->getCurrentPosition();
	position posY = limSteppers[eStepperIdx_Y]->getCurrentPosition();

	position diff = posX - posY;
	limSteppers[eStepperIdx_Y]->shift((position){diff.x, 0, 0});

	Gui::glSelectColor(eColor_blue);
	limSteppers[eStepperIdx_Y]->draw();

	Gui::glSelectColor(eColor_red);
	limSteppers[eStepperIdx_Z]->draw();

	Gui::glSelectColor(eColor_black);

	position C;
	if(_getEndpoint(C))
	{
		Gui::glSelectColor(eColor_red);

		for (int i = 1; i < 5; i+=1)
		{
			drawCircle(C, i);
		}

		Gui::glSelectColor(eColor_black);
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


void addPointToDrawList(void)
{
    position C;

    if (_getEndpoint(C))
    {
   		static position currentPos = C;

    	guiCommand outCmd;
		outCmd.extrudeLength = 1;
		outCmd.startPosition = currentPos;
		outCmd.endPosition = C;

		std::lock_guard<std::mutex> hold(drawList_lock);

		if (currentPos.z != C.z)
		{
			LOG("New layer started");
			drawList.erase(drawList.begin(), drawList.end());
		}

		drawList.push_back(outCmd);
		currentPos = C;
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
        LimStepperGui * stepper = limSteppers[devId];
    	stepper->setDirection(true);
        break;
    }
    case ePin_Step:
    {
    	LimStepperGui * stepper = limSteppers[devId];
    	stepper->move();
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
        LimStepperGui * stepper = limSteppers[devId];
    	stepper->setDirection(false);
        break;
    }
    case ePin_Step:
	{
		addPointToDrawList();
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
        LimStepperGui * stepper = limSteppers[devId];
    	return stepper->isAtLimitPosition();
        break;
    }

    case ePin_LimSw2:
    {
        LimStepperGui * stepper = limSteppers[devId];
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
	position p1 = {-10, 0, 0};
	position p2 = {10, 0, 0};

	position p3 = {0, -10, 0};
	position p4 = {0, 10, 0};

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

#endif // PRINTER_TYPE == PRINTER_TYPE_3D_PRINTER
