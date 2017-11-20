/*
 * math_tools.h
 *
 *  Created on: 26. 2. 2017
 *      Author: apollo
 */

#ifndef _MATH_TOOLS_H_
#define _MATH_TOOLS_H_

float getDistance(position A, position B);
float getDistance3D(position A, position B);

bool getIntersection(position A, float r1, position B, float r2, position &inter1, position &inter2);
float getAngle(position A, position B);

int32_t angleToRelative(float angle);
float relativeToAngle(int32_t relative);

int32_t zAxeToRelative(float zPosition);
float relativeToZAxe(int32_t relative);

#endif /* _MATH_TOOLS_H_ */
