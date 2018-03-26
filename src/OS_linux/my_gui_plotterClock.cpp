/*
 * my_gui_plotterClock.cpp
 *
 *  Created on: 24. 3. 2018
 *      Author: apollo
 */
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

class ServoGui
{
    position endPoint;
    position rotationCenter;
    float angle;
    float filteredAngle;
    float Kfilter;
    int armLength;
    int armAngleOffset;

    float minAngle;
    float maxAngle;

public:
    ServoGui(position _rotationCenter, int _armLength, int _armAngleOffset)
    :endPoint({0, 0, 0}),
    rotationCenter(_rotationCenter),
    angle(0),
    filteredAngle(angle),
    Kfilter(1),
    armLength(_armLength),
    armAngleOffset(_armAngleOffset),
    minAngle(MIN_ANGLE),
    maxAngle(MAX_ANGLE)
    {
       this->update(this->angle);
    };

    void draw(void)
    {
        updateFilter();
        drawCircle(getCirclePosition(rotationCenter, armLength, 0), 2);
        drawCircle(getCirclePosition(rotationCenter, armLength, 90), 2);
        drawCircle(getCirclePosition(rotationCenter, armLength, 180) , 2);

        drawCircle(getCirclePosition(rotationCenter, armLength, 45), 2);
        drawCircle(getCirclePosition(rotationCenter, armLength, 135) , 2);


        position needle = getCirclePosition(rotationCenter, 30, this->angle);
        drawLine(rotationCenter, needle);

        drawLine(rotationCenter, endPoint);
        drawCircle(rotationCenter, 10);
    }

    position getEndPoint(void)
    {
        updateFilter();
        return this->endPoint;
    }


    float getAngle(void)
    {
        updateFilter();
        return this->angle;
    }

    void updateFilter(void)
    {
        filteredAngle = ((1 - this->Kfilter) * filteredAngle) + (this->Kfilter * angle);
        endPoint = getCirclePosition(rotationCenter, armLength, this->filteredAngle + (float)armAngleOffset);
    }

    void update(float angle)
    {
        this->angle = constrain(angle, this->minAngle, this->maxAngle);
        updateFilter();
    }
};

ServoGui servo1(pos_S1, armLength_AS1, LEFT_ARM_OFFSET);
ServoGui servo2(pos_S2, armLength_BS2, RIGHT_ARM_OFFSET);
ServoGui servo3(pos_S1, 0, 0);

ServoGui * servo[] = {&servo1, &servo2, &servo3};

static uint32_t cServoCount = ARRAY_SIZE(servo);

void servoInit()
{
    return;
}

// Forward declaration
void addPointToDrawList(void);

void servoSetPosition(float angle, uint32_t channel)
{
    if (channel < cServoCount)
    {
        float tmpAngle = angle;//((int)(angle * 2))/2.0;
        servo[channel]->update(tmpAngle);
    }

    if (channel == cServoCount - 1)
    {
        addPointToDrawList();
    }
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

// ----------------------------------------------------------------------------

// HW simulation

bool _getEndpoint(position& C)
{
    position A = servo[0]->getEndPoint();
    position B = servo[1]->getEndPoint();
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

bool _getPositionOfPenholder(position B, position C, position& D)
{
    position D1, D2;

    if (getIntersection(C, armLength_AD, B, armLength_BD, D1, D2))
    {
        float dist1 = getDistance(D1, pos_S1);
        float dist2 = getDistance(D2, pos_S2);

        D = (dist1 < dist2)? D2 : D1;
        return true;
    }
    else
    {
        return false;
    }
}



void drawSteppers()
{
    // Draw stepper
    Gui::glSelectColor(eColor_green);
    servo[0]->draw();

    Gui::glSelectColor(eColor_blue);
    servo[1]->draw();

    Gui::glSelectColor(eColor_black);

    position A = servo[0]->getEndPoint();
    position B = servo[1]->getEndPoint();

    // Calculate position of the point C
    position C;
    if(!_getEndpoint(C))
    {
    	LOG("Point C not found");
    	return;
    }

    // Calculate position of the point D
    position D;
    if(!_getPositionOfPenholder(B, C, D))
    {
    	LOG("Point D not found");
    	return;
    }

    Gui::glSelectColor(eColor_black);

    drawLine(A, C);
    drawLine(B, C);
    drawLine(C, D);
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
    /*              D
     *             /
     *           C/
     *           /\
     *          /  \
     *         /    \
     *        /      \
     *       A        B
     *        \      /
     *         \    /
     *          \  /
     *          S1 S2
     *
     */

    // C = Required position

    position S1 = pos_S1;
    position S2 = pos_S2;
    position A = servo[0]->getEndPoint();
    position B = servo[1]->getEndPoint();
    bool extrude = (servo[2]->getAngle() > 90) ? true : false;

    // Calculate position of the point C
    position C;
    if(!_getEndpoint(C))
    {
    	LOG("Point C not found");
    	return;
    }

    // Calculate position of the point D
    position D;
    if(!_getPositionOfPenholder(B, C, D))
    {
    	LOG("Point D not found");
    	return;
    }

    static position currentPos = D;
/*  static position tmpC = C;


	float k = 1;

	tmpC.x = k * C.x + (1 - k) * tmpC.x;
	tmpC.y = k * C.y + (1 - k) * tmpC.y;
	tmpC.z = k * C.z + (1 - k) * tmpC.z;
*/

	guiCommand outCmd;
	outCmd.extrudeLength = extrude;
	outCmd.startPosition = currentPos;
	outCmd.endPosition = D;

	currentPos = D;

	std::lock_guard<std::mutex> hold(drawList_lock);
	drawList.push_back(outCmd);
}
//////////////////////////////////////////////////////////

void update(void)
{
    Gui::clear();

    if (1)
    {
        Gui::drawLinesStart();
        std::lock_guard<std::mutex> hold(drawList_lock);
        for (auto command : drawList)
        {
            eColor color = (command.extrudeLength > 0)? eColor_blue : eColor_red;
            Gui::glSelectColor(color);
            Gui::drawLines(center(command.startPosition));
            Gui::drawLines(center(command.endPosition));
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
    switch(x)
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

//// Dummy GPIOs

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

Gpio::Gpio(GpioDesc_t gpioDesc){}

void Gpio::powerUp ()
{}

void Gpio::turnOn ()
{}

void Gpio::turnOff ()
{}

void Gpio::toggle ()
{}

bool Gpio::isOn ()
{
    return false;
}

bool Gpio::isValid()
{
    return false;
}

