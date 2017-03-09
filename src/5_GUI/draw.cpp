/*
 * draw.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: apollo
 */

#include <math.h>
#include <unistd.h>
#include "draw.h"
#include "gui.h"

float zoom = 5;

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

    glLineWidth(2);
    //glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2f((float)start.x /  windowSize.x, 1 - ((float)start.y / windowSize.y));
    glVertex2f((float)end.x / windowSize.x, 1 - ((float)end.y / windowSize.y));
    glEnd();

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


static float distPtSeg(float x, float y, float px, float py, float qx, float qy)
{
	float pqx, pqy, dx, dy, d, t;
	pqx = qx-px;
	pqy = qy-py;
	dx = x-px;
	dy = y-py;
	d = pqx*pqx + pqy*pqy;
	t = pqx*dx + pqy*dy;
	if (d > 0) t /= d;
	if (t < 0) t = 0;
	else if (t > 1) t = 1;
	dx = px + t*pqx - x;
	dy = py + t*pqy - y;
	return dx*dx + dy*dy;
}

void drawCubicBez(float x1, float y1, float x2, float y2,
                  float x3, float y3, float x4, float y4,
                  float tol, int level)
{
	float x12,y12,x23,y23,x34,y34,x123,y123,x234,y234,x1234,y1234;
	float d;

	if (level > 12)
	{
		drawLine(x1, y1, x4, y4);
		return;
	}

	x12 = (x1+x2)*0.5f;
	y12 = (y1+y2)*0.5f;
	x23 = (x2+x3)*0.5f;
	y23 = (y2+y3)*0.5f;
	x34 = (x3+x4)*0.5f;
	y34 = (y3+y4)*0.5f;
	x123 = (x12+x23)*0.5f;
	y123 = (y12+y23)*0.5f;
	x234 = (x23+x34)*0.5f;
	y234 = (y23+y34)*0.5f;
	x1234 = (x123+x234)*0.5f;
	y1234 = (y123+y234)*0.5f;

	d = distPtSeg(x1234, y1234, x1,y1, x4,y4);
	if (d > tol*tol) {
		drawCubicBez(x1,y1, x12,y12, x123,y123, x1234,y1234, tol, level+1);
		drawCubicBez(x1234,y1234, x234,y234, x34,y34, x4,y4, tol, level+1);
	} else {
		drawLine(x1, y1, x2, y2);
		drawLine(x2, y2, x3, y3);
		drawLine(x3, y3, x4, y4);
		//drawLine(x1, y1, x2, y2);
		//glVertex2f(x4, y4);
	}
}


