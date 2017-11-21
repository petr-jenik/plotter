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

float zoom = 10;

position center(position pos)
{
    position windowSize = Gui::getWindowSize();

    position result;

    result.x = (windowSize.x / 2) + zoom * pos.x;
    result.y = (windowSize.y / 2) + zoom * pos.y;
    result.z = zoom * pos.z;
    return result;
}

float toRads(float angle)
{
    return (angle * 2 * M_PI)/360;
}

void drawLine(position _start, position _end)
{
    position windowSize = Gui::getWindowSize();
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

position getCirclePosition(position center, int radius, float angle)
{
    float fRadius = (float) radius;

    position pos;

    pos.x = center.x + fRadius * cos(toRads(angle));
    pos.y = center.y + (fRadius * sin(toRads(angle)));
    pos.z = 0;

    return pos;
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




