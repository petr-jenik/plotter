/*
 * draw.h
 *
 *  Created on: Jan 20, 2016
 *      Author: apollo
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "gui_types.h"

void drawLine(position start, position end);
//void drawLine(int x1, int y1, int x2, int y2);
position getCirclePosition(position center, int radius, float angle);
void drawCubicBez(float x1, float y1, float x2, float y2,
                  float x3, float y3, float x4, float y4,
                  float tol, int level);
//void drawCircle(position center, int radius, int numberOfSteps = 20);

#ifdef __cplusplus
}
#endif
