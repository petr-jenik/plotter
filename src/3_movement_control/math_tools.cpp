/*
 * math_tool.cpp
 *
 *  Created on: 26. 2. 2017
 *      Author: apollo
 */

#include <cmath>
#include "global.h"
#include "config.h"

#include <iostream>

float getDistance(position A, position B)
{
    float distance = pow((pow(A.x - B.x,2) + pow(A.y - B.y, 2)/* + pow(A.z - B.z, 2)*/), 0.5);
    return distance;
}

float getAngle(position A, position B)
{
	float dx = B.x - A.x;
	float dy = B.y - A.y;

	float angle = atan2(dx,dy);
	return ((angle * 360.0) / (2 * M_PI));
}

bool getInterception(position A, float r1, position B, float r2, position &inter1, position &inter2)
{
    float d = getDistance(A,B);
    if (d > (r1 + r2))
    {
        //std::cout << "end points are too far" << std::endl;
        return false;
    }


    if (abs(d) < cMinimalDistanceToCenter)
    {
        //std::cout << "end points are too close" << std::endl;
        return false;
    }

    float  m = (float) ((d +1) / 2.0) + ((pow(r1, 2) - pow(r2, 2)) / ( 2.0 * d ));
    float v = pow(abs(pow(r1, 2) - pow(m, 2)), 0.5);

    float f_x = (float) A.x + (float)(((float)(B.x - A.x)*m)/d);// + (float)(v/d));
    float f_y = (float) A.y + (float)(((float)(B.y - A.y)*m)/d);
    //+ (float)(v/d));

    float f_x_1 = f_x;
    float f_y_1 = f_y;
    float f_x_2 = f_x;
    float f_y_2 = f_y;

    f_x_1 += (v/d) * (float)(A.y - B.y);
    f_y_1 -= (v/d) * (float)(A.x - B.x);

    f_x_2 -= (v/d) * (float)(A.y - B.y);
    f_y_2 += (v/d) * (float)(A.x - B.x);


    inter1.x = f_x_1;
    inter1.y = f_y_1;

    inter2.x = f_x_2;
    inter2.y = f_y_2;

    // TODO Fix this
    inter1.z = 0;
    inter2.z = 0;

    return true;
}

