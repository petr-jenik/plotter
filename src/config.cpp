/*
 * config.cpp
 *
 *  Created on: 27. 2. 2017
 *      Author: apollo
 */



#include "config.h"

#include "global.h"


std::ostream& operator<< (std::ostream& stream, position pos)
{
	stream << "[" << pos.x << "," << pos.y << "," << pos.z << "]";
	return stream;
}

std::ostream& operator << (std::ostream& stream, const stepperCommand& cmd)
{
	//stream << cmd.
	return stream;
}

std::ostream& operator << (std::ostream& stream, const moveCommand& cmd)
{
	stream << "From " << cmd.pos1;
	stream << " to " << cmd.pos2;
	return stream;
}


std::ostream& operator << (std::ostream& stream, const armCommand& cmd)
{
	stream << "[" << cmd.relativeAngle1 << ", " << cmd.relativeAngle2 << ", " << cmd.relPosZ << ", " << cmd.extrude << "]";
	return stream;
}

position pos_S1 = {0, -100, 0};
position pos_S2 = {0, -100, 0};

const float maxRelativeAngle = 10000000.0;

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


float zAxeMin = 0;
float zAxeMax = 1000;
const float maxRelativeZ = 100000.0;

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
