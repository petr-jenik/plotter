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

/*
 * Return distance between point A and B (works only for 2D)
 * @param[in] position A
 * @param[in] position B
 * @return float distance
 */
float getDistance(position A, position B)
{
    float distance = pow((pow(A.x - B.x,2) + pow(A.y - B.y, 2)/* + pow(A.z - B.z, 2)*/), 0.5);
    return distance;
}


/*
 * Return distance between point A and B (works even for 3D)
 * @param[in] position A
 * @param[in] position B
 * @return float distance
 */
float getDistance3D(position A, position B)
{
    float distance = pow((pow(A.x - B.x,2) + pow(A.y - B.y, 2) + pow(A.z - B.z, 2)), 0.5);
    return distance;
}

// line_between center of steppers and position {0, 0}
position line_start = {(pos_S1.x + pos_S2.x)/2, (pos_S1.y + pos_S2.y)/2};
position line_end = {0,0};

float radsToDegs(float radians)
{
	return (radians * 180) / M_PI;
}

/*
 * Get angle between two points
 *
 * A
 * o  B
 * |  o
 * |a/
 * |/
 *
 *@param[in] position A
 *@param[in] position B
 *@return float angle
 */
float getAngle(position A, position B)
{
#warning "TODO Fix this broken function!!"
	//TODO tato funkce vraci spatne vysledky - opravit!!! Pokud je cilovy bod {0,0,0} uhel by mel byt asi 45°.


	float a = getDistance({0,0}, A);
	float b = getDistance({0,0}, B);
	float c = getDistance(A, B);

	float p = (a + b + c)/2;

	float vb = (2/b)* pow((p*(p-a)*(p-b)*(p-c)), 0.5);
	float angle = asin(vb/a);

	return radsToDegs(angle);
//	float slope1 = (line_start.y - line_end.y)/(line_start.x - line_end.x);
//	float slope2 = (A.y - B.y)/(A.x - B.x);

//	float angle = atan((slope1 - slope2) / (1+slope1*slope2));
//	return angle;

	//float angle1 = radsToDegs(atan2((line_start.x - line_end.x),(line_start.y - line_end.y)));
	//sfloat angle2 = radsToDegs(atan2((A.x - B.x),(A.y - B.y)));
	//return 180 - abs(angle2 - angle1);
	//float dx = B.x - A.x;
	//float dy = B.y - A.y;

	//float angle = atan2(dx,dy);
	//return ((angle * 360.0) / (2 * M_PI));
}


/*
 * Find intersection between 2 circles
 * First circle - center A, radius r1
 * Second circle - center B, radius r2
 *
 *@param[in] position A
 *@param[in] r1 - arm length from position A
 *@param[in] position B
 *@param[in] r2 - arm length from position B
 *@param[out] position inter1
 *@param[out] positoon inter2
 *@return True if intersection was found, else False
 */
bool getIntersection(position A, float r1, position B, float r2, position &inter1, position &inter2)
{
    float d = getDistance(A,B);
    if (d > (r1 + r2))
    {
        std::cout << "end points are too far" << std::endl;
        return false;
    }


    if (abs(d) < cMinimalDistanceToCenter)
    {
        //std::cout << "end points are too close" << A << B << std::endl;
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


/*
 * Converts zPosition to relative position in range 0 - 100000;
 */
int32_t zAxeToRelative(float zPosition)
{
	int32_t relative = (maxRelativeZ * (zPosition - zAxeMin))/(zAxeMax - zAxeMin);
	return relative;
}


float relativeToZAxe(int32_t relative)
{
	float zPosition = ((relative/maxRelativeZ) * (zAxeMax - zAxeMin)) + zAxeMin;
	return zPosition;
}


int32_t angleToRelative(float angle)
{
	//angle += 180;
	return (angle * maxRelativeAngle) / 180.0;
}


float relativeToAngle(int32_t relative)
{
	float angle = (relative * 180.0) / maxRelativeAngle;
	//angle -= 90;
	return angle;
}

