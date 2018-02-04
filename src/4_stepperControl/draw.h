/*
 * draw.h
 *
 *  Created on: Jan 20, 2016
 *      Author: apollo
 */

#ifndef DRAW_H_
#define DRAW_H_



#include "gui.h"

#include "global.h"


void increaseZoom();
void decreaseZoom();
void setCenterPoint(position newCenterPoint);
void drawLine(position start, position end);
void drawLine(int x1, int y1, int x2, int y2);
position getCirclePosition(position center, int radius, float angle);
void drawCircle(position center, int radius, int numberOfSteps = 20);

#endif // DRAW_H_
