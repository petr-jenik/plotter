/*
 * math_tools.h
 *
 *  Created on: 26. 2. 2017
 *      Author: apollo
 */

#ifndef _MATH_TOOLS_H_
#define _MATH_TOOLS_H_

#include "global.h"

float getDistance(position A, position B);
float getDistance3D(position A, position B);

bool getIntersection(position A, float r1, position B, float r2, position &inter1, position &inter2);

bool getIntersectionCloserToRefPoint(const position A,
									const float r1,
									const position B,
									const float r2,
									const position referencePoint,
									position& resultPoint);

bool getIntersectionFartherToRefPoint(const position A,
									const float r1,
									const position B,
									const float r2,
									const position referencePoint,
									position& resultPoint);

float getAngle(position A, position B, position C);

//int32_t angleToRelative(float angle);
//float relativeToAngle(int32_t relative);

int32_t zAxeToRelative(float zPosition);
float relativeToZAxe(int32_t relative);

position getCirclePosition(position center, float radius, float angle);

float toPositiveAngle(float angle);


#endif /* _MATH_TOOLS_H_ */
