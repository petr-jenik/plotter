/*
 * my_gui_plotterClock.cpp
 *
 *  Created on: 24. 3. 2018
 *      Author: apollo
 */

#include "project_config.h"
#if PRINTER_TYPE == PRINTER_TYPE_2D_PLOTTER_WITH_SERVOS
#include <mutex>
#include <vector>

#include "draw.h"
#include "gui.h"
#include "math_tools.h"
#include "hwGpio.h"
#include "stepperSim.h"
#include "math_tools.h"
#include "global.h"
#include "hwStepperPins.h"
#include "hwServo.h"
#include "servo.h"

//#include <stdio.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <string.h>


bool udpSend(char * data, size_t dataLen)
{
	static bool udpInitialized = false;
	static int sock = -1;
	static struct sockaddr_in serverAddr;

	if (udpInitialized == false)
	{
		//Send to
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(7777);

		sock = socket(PF_INET, SOCK_DGRAM, 0);
		if (sock == -1)
		{
			LOG("socket error");
			return false;
		}
		LOG("socket OK");
		LOG(sock);

		udpInitialized = true;
	}

	int bytesCount = sendto(sock,
							data,
							dataLen,
							0,
							(struct sockaddr *) &serverAddr,
							sizeof(serverAddr));

	if (bytesCount == -1)
	{
		LOG("error: sendto");
		while(1);
		return false;
	}

	return true;
}


class ServoGui
{
    position endPoint;
    position rotationCenter;
    float armAngle;
    float servoAngle;
    float filteredAngle;
    float Kfilter;
    int armLength;
    int armAngleOffset;

    float minAngle;
    float maxAngle;
    bool reverseAngle;

public:
    ServoGui(position _rotationCenter,
    		 int _armLength,
			 int _armAngleOffset,
			 bool _reverseAngle)
    :endPoint({0, 0, 0}),
    rotationCenter(_rotationCenter),
    servoAngle(0),
	//requiredAngle(0),
    filteredAngle(armAngle),
    Kfilter(1),
    armLength(_armLength),
    armAngleOffset(_armAngleOffset),
    minAngle(MIN_ANGLE),
    maxAngle(MAX_ANGLE),
	reverseAngle(_reverseAngle)
    {
       this->update(this->armAngle);
    };

    void draw(void)
    {
        updateFilter();
        drawCircle(getCirclePosition(rotationCenter, armLength, 0), 2);
        drawCircle(getCirclePosition(rotationCenter, armLength, 90), 2);
        drawCircle(getCirclePosition(rotationCenter, armLength, 180) , 2);

        drawCircle(getCirclePosition(rotationCenter, armLength, 45), 2);
        drawCircle(getCirclePosition(rotationCenter, armLength, 135) , 2);


        position needle = getCirclePosition(rotationCenter, 30, getAngle());
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
        return this->filteredAngle;
    }

    void updateFilter(void)
    {
		this->filteredAngle = ((1 - this->Kfilter) * filteredAngle) + (this->Kfilter * this->armAngle);
		endPoint = getCirclePosition(rotationCenter,
									armLength,
									this->filteredAngle);
    }

    void update(float angle)
    {
    	LOG("Angle:" << angle);
    	this->servoAngle = angle;
    	//this->requiredAngle = angle;//templateConstrain(angle, this->minAngle, this->maxAngle);

    	if (reverseAngle == true)
    	{
			this->armAngle = 180 - (this->armAngleOffset - angle);
		}
		else
		{
			this->armAngle = angle -this->armAngleOffset;
		}
    	LOG("ArmAngle:" << armAngle);
        updateFilter();
    }
};


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
 *  right - S1 S2 - left
 *
 */

// Servo motor in position S1 is right servo
// Servo motor in position S2 is left servo

ServoGui servoRight(pos_S1, armLength_AS1, RIGHT_ARM_OFFSET, false);
ServoGui servoLeft(pos_S2, armLength_BS2, LEFT_ARM_OFFSET, true);
ServoGui servoZ(pos_S1, 0, 0, false);

ServoGui * servo[] = {&servoRight, &servoLeft, &servoZ};
//static_assert(servo[eIdxRight] ==  servoRight, "test");
//static_assert(servo[eIdxLeft] ==  servoLeft, "test");

static uint32_t cServoCount = ARRAY_SIZE(servo);

void hwServoInit(int32_t channel, float defaultAngle)
{
    return;
}

// Forward declaration
void addPointToDrawList(void);

void sendAnglesToDiag(void)
{
	static int cycleCount = 0;
	cycleCount++;
	char message[512] = {0};
	snprintf(message,  sizeof(message),
			"start %f %f %f %d",
			servo[0]->getAngle(),
			servo[1]->getAngle(),
			servo[2]->getAngle(),
			cycleCount);
	udpSend(message, strlen(message));

}


void hwServoSetPosition(float angle, uint32_t channel)
{
    if (channel < cServoCount)
    {
        DBG("channel: " << channel << ", angle: " << (int) angle);
        //= angle;//((int)(angle * 2))/2.0;
        float tmpAngle = templateConstrain(angle, 0.0f, 180.0f);
        servo[channel]->update(tmpAngle);
    }
    else
    {
    	DBG("Fail in file " << __FILE__ << " on line " << __LINE__);
    	new_assert(false);
    }

    if (channel == cServoCount - 1)
    {
    	sendAnglesToDiag();
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

static bool _getEndpoint(position& C)
{
    position A = servo[eIdxRight]->getEndPoint();
    position B = servo[eIdxLeft]->getEndPoint();

	return getIntersectionFartherToRefPoint(A,
											armLength_AC,
											B,
											armLength_BC,
											pos_S1,
											C);
}

static bool _getPositionOfPenholder(position B, position C, position& D)
{
	return getIntersectionFartherToRefPoint(B,
											armLength_BD,
											C,
											armLength_CD,
											pos_S2,
											D);
}

void drawSteppers()
{
    // Draw stepper

	//Right
    Gui::glSelectColor(eColor_green);
    servo[eIdxRight]->draw();

    // Left = B = blue
    Gui::glSelectColor(eColor_blue);
    servo[eIdxLeft]->draw();

    Gui::glSelectColor(eColor_black);

    position A = servo[eIdxRight]->getEndPoint();
    position B = servo[eIdxLeft]->getEndPoint();

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
    bool extrude = (servo[eIdxZ]->getAngle() > 90) ? false : true;

    // Calculate position of the point C
    position C;
    if(!_getEndpoint(C))
    {
    	LOG("Point C not found");
    	return;
    }

    // Calculate position of the point D
    position D;
    position B = servo[eIdxLeft]->getEndPoint();

    if(!_getPositionOfPenholder(B, C, D))
    {
    	LOG("Point D not found");
    	return;
    }

    static position currentPos = D;

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

            if (command.extrudeLength <= 0)
            {
            	continue;
            }

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

    servo[eIdxRight] = &servoRight;
    servo[eIdxLeft] = &servoLeft;
    servo[2] = &servoZ;

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


#endif // PRINTER_TYPE == PRINTER_TYPE_2D_PLOTTER
