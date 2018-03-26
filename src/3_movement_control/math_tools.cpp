/*
 * math_tool.cpp
 *
 *  Created on: 26. 2. 2017
 *      Author: apollo
 */

#include <cmath>
#include "global.h"
#include "config.h"

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


float radsToDegs(float radians)
{
    return ((radians * 180.0) / M_PI);
    //TODO add modulo 360
}


float degsToRads(float degs)
{
    return (degs * M_PI) / 180;
}


position operator-(position &val1, position &val2)
{
    position retval = {val1.x - val2.x, val1.y - val2.y, val1.z - val2.z};
    return retval;
}


position operator+(position val1, position &val2)
{
    position retval = {val1.x + val2.x, val1.y + val2.y, val1.z + val2.z};
    return retval;
}


float toPositiveAngle(float angle)
{
    //return angle;
    while(angle < 0)
    {
        angle += 360;
    }

    int count = angle / 360;
    return angle - 360.0 * count;
}


/*
 *    C
 *   /
 *  / angle
 * A-----B
 *@param[in] position A
 *@param[in] position B
 *@param[in] position C
 *@return float angle
 */
float getAngle(position pos_A, position pos_B, position pos_C)
{
    // A - base start - rotation center
    position dA = pos_A - pos_A;
    position dB = pos_B - pos_A;
    position dC = pos_C - pos_A;

    float angle1 = radsToDegs(atan2(dC.y, dC.x));
    float angle2 = radsToDegs(atan2(dB.y, dB.x));

    return toPositiveAngle(angle1 - angle2);
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
bool getIntersection(position A,
                    float r1,
                    position B,
                    float r2,
                    position &inter1,
                    position &inter2)
{
    float d = getDistance(A,B);
    if (d > (r1 + r2))
    {
        LOG("end points are too far");
        while(1)
        return false;
    }

    if (template_ABS(d) < cMinimalDistanceToCenter)
    {
        LOG("end points are too close" << A << B);
        while(1)
        return false;
    }

    float  m = (d /2.0) + ((pow(r1, 2) - pow(r2, 2)) / ( 2.0 * d ));
    float v = pow(template_ABS(pow(r1, 2) - pow(m, 2)), 0.5);

    float f_x = A.x + (((B.x - A.x)*m)/d);
    float f_y = A.y + (((B.y - A.y)*m)/d);
    //+ (float)(v/d));

    /*
    float f_x_1 = f_x;
    float f_y_1 = f_y;
    float f_x_2 = f_x;
    float f_y_2 = f_y;

    f_x_1 += (v/d) * (float)(A.y - B.y);
    f_y_1 -= (v/d) * (float)(A.x - B.x);

    f_x_2 -= (v/d) * (float)(A.y - B.y);
    f_y_2 += (v/d) * (float)(A.x - B.x);
*/

    inter1.x = f_x + ((v/d) * (A.y - B.y));
    inter1.y = f_y - ((v/d) * (A.x - B.x));

    inter2.x = f_x - ((v/d) * (A.y - B.y));
    inter2.y = f_y + ((v/d) * (A.x - B.x));

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
    new_assert(relative >= 0);
    return relative;
}


float relativeToZAxe(int32_t relative)
{
    float zPosition = ((relative/maxRelativeZ) * (zAxeMax - zAxeMin)) + zAxeMin;
    return zPosition;
}

/*

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
*/

position getCirclePosition(position center, float radius, float angle)
{
    position pos;

    pos.x = center.x + radius * cos(degsToRads(angle));
    pos.y = center.y + (radius * sin(degsToRads(angle)));
    pos.z = center.z;

    return pos;
}

