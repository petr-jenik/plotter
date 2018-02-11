/*
 * draw.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: apollo
 */

#include "draw.h"
#include <math.h>
#include "gui.h"

#include "global.h"
#include "math_tools.h"

float zoom = 20;

position gOffsetPoint = {0, 0};


void increaseZoom()
{
	zoom *= 1.5;
}


void decreaseZoom()
{
	if (zoom > 1.5)
	{
		zoom /= 1.5;
	}
}


void setCenterPoint(position newCenterPoint)
{

    position windowSize = Gui::getWindowSize();

	position point;
	point.x = ((newCenterPoint.x - (windowSize.x/2)) / zoom) + gOffsetPoint.x;
	point.y = ((newCenterPoint.y - (windowSize.y/2)) / zoom) + gOffsetPoint.y;
	cout << point << endl;
	gOffsetPoint = point;
}

position center(position pos)
{
    position windowSize = Gui::getWindowSize();

    position result;
    result.x = (windowSize.x / 2) + zoom * (pos.x - gOffsetPoint.x);
    result.y = (windowSize.y / 2) + zoom * (pos.y - gOffsetPoint.y);
    result.z = zoom * pos.z;
    return result;
}


void drawLine(position _start, position _end)
{
    //position windowSize = Gui::getWindowSize();
    //Gui::clear();

    //Gui::forceRedraw();

    position start = center(_start);
    position end = center(_end);

    Gui::drawLine(start, end);

    //Gui::forceRedraw();

    //usleep(10);

    //Gui::forceRedraw();


    //usleep(10);
}

void drawLine(int x1, int y1, int x2, int y2)
{
    position start;
    position end;

    start.x = x1;
    start.y = y1;

    end.x = x2;
    end.y = y2;

    drawLine(start, end);
}


void drawCircle(position center, int radius, int numberOfSteps /* = 20*/)
{
    position actualPosition = center;
    actualPosition.x += radius;

    float step = (float) 360 / numberOfSteps;
    float angle = 0;
    for (int i = 0; i < numberOfSteps; i++)
    {
        angle += step;
        position newPosition = getCirclePosition(center, radius, angle);
        drawLine(actualPosition, newPosition);
        actualPosition = newPosition;
    }
}




