/*
 * math_tools.h
 *
 *  Created on: 26. 2. 2017
 *      Author: apollo
 */

#ifndef _MATH_TOOLS_H_
#define _MATH_TOOLS_H_

float getDistance(position A, position B);
bool getInterception(position A, float r1, position B, float r2, position &inter1, position &inter2);
float getAngle(position A, position B);
#endif /* _MATH_TOOLS_H_ */
