/*
 * movement_control.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include "math_tools.h"
#include "project_config.h"
#include "global.h"
#include "stepperConfig.h"

#include "stepperControl_main.h"
#include "movementControl_main.h"
#include "Timer.h"


#ifdef OS_LINUX
#include "my_gui.h"
#endif

position gCurrentPosition = {0,0,0};

void movementControl_createLine(position finalPosition,
									float extrudeLength,
									float movementSpeed)
{
	TRACE; // Trace macro

	// Flip image along the X axis
	finalPosition.y *= -1;

	position startPos = gCurrentPosition;

#ifdef OS_LINUX
	guiCommand cmd = {1, 1, gCurrentPosition, finalPosition};
	gui_add_line(cmd, eColor_green);
#endif //OS_LINUX

	// get distance between start and end points
	float distance = getDistance3D(startPos, finalPosition);

	if (distance == 0)
	{
		//LOG("distance = 0");
		return;
	}

	#define deltaX (finalPosition.x - startPos.x)
	#define deltaY (finalPosition.y - startPos.y)
	#define deltaZ (finalPosition.z - startPos.z)

	// A close approximation to a straight line between two points
	int numberOfSteps = (int)(SPEED_MAGICAL_CONSTANT*distance);

	// Do next loop at least once.
	if (0 == numberOfSteps)
	{
		numberOfSteps = 1;
	}

	//LOG("numberOfSteps: " << numberOfSteps);

	for (float i = 0; i < numberOfSteps+1; i++)
	{
		position currentPos;
		currentPos.x = startPos.x + (deltaX * i)/numberOfSteps;
		currentPos.y = startPos.y + (deltaY * i)/numberOfSteps;
		currentPos.z = startPos.z + (deltaZ * i)/numberOfSteps;

		stepperControl_goToThisPosition(currentPos,
				                        extrudeLength / numberOfSteps,
										movementSpeed);
		gCurrentPosition = currentPos;

		// Useless delay used for a kicking of the WDT
		Timer::sleep(0);

	}
}


void movementControl_createLine(const moveCommand& cmd)
{
	TRACE; // Trace macro

	movementControl_createLine(cmd.finalPosition, cmd.extrudeLength, cmd.movementSpeed);
}


moveCommand getSquearePoints(int idx, float size, position center)
{
	TRACE; // Trace macro

	idx = (idx < 0)? 0 : idx;
	idx = (idx > 4)? 4 : idx;

	float speed = 1;
	float extrudeLength = size;

	moveCommand squareCommands[] =
	{
		// First moveCommand simulates "move to" command
		{0            , speed, {center.x - size/2, center.y - size/2, center.z}},
		{extrudeLength, speed, {center.x + size/2, center.y - size/2, center.z}},
		{extrudeLength, speed, {center.x + size/2, center.y + size/2, center.z}},
		{extrudeLength, speed, {center.x - size/2, center.y + size/2, center.z}},
		{extrudeLength, speed, {center.x - size/2, center.y - size/2, center.z}}
	};

	return squareCommands[idx];
}


void printLine(position startPos, position endPos)
{
	TRACE; // Trace macro

	moveCommand cmd = {0, 1 , startPos};
	movementControl_createLine(cmd);
    cmd = {1, 1 , endPos};
	movementControl_createLine(cmd);
}

void printRectangle(float size, position center)
{
	TRACE; // Trace macro

	for (int idx = 0; idx < 5; idx++)
	{
		moveCommand cmd = getSquearePoints(idx, size, center);
		movementControl_createLine(cmd);
	}
}


void printCircle(float radius, position center)
{
	TRACE; // Trace macro

	int numberOfSteps = 36;
	for (int i = 0; i < numberOfSteps + 1; i++)
	{
		float angle = (float)(360 * i) / (float)(numberOfSteps);
		position pos = getCirclePosition(center, radius, angle);

		moveCommand cmd = {(i == 0)?0:1 /*extrudeLength*/, 1 /*speed*/, pos};
		movementControl_createLine(cmd);
	}
}



static void showDemo()
{
	TRACE; // Trace macro

	printRectangle(50, {0, 0, 0});

	//printLine({0,0,0}, {0,100, 0});
	/*
	for (int i = 0; i < 30; i += 1)
	{
		printRectangle(2*i, {0,0,0});
		//printLine({-150, i, 0}, {150, i, 0});
		printCircle(i, { 0, 0, 0});
	}*/
	/*
	printCircle(10, {-100, 100, 0});
	printCircle(10, {-100, -100, 0});
	printCircle(10, {100, -100, 0});
	*/
}

void movementControl_showDemo(void)
{
	TRACE; // Trace macro
	while(1)
	{
		showDemo();
	}
}

void movementControl_init()
{
	TRACE; // Trace macro

	// Go to position {0, 0, 0}
	stepperControl_goToThisPosition({0, 0, 0} /*position*/, 0/*extrudeLength*/, 1/*movementSpeed*/ );
}
