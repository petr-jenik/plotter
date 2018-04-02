/*
 * dataTypesToStream.cpp
 *
 *  Created on: 16 Mar 2018
 *      Author: pi
 */


#include "global.h"

EmbeddedCout& operator<<(EmbeddedCout& stream, position pos)
{
	stream << "[" << pos.x << "," << pos.y << "," << pos.z << "]";
	return stream;
}


EmbeddedCout& operator << (EmbeddedCout& stream, const moveCommand& cmd)
{
	stream << "Go to: " << cmd.finalPosition;
	stream << ", extrude: " << cmd.extrudeLength;
	stream << ", movementSpeed: " << cmd.movementSpeed;
	return stream;
}


EmbeddedCout& operator << (EmbeddedCout& stream, const armCommand& cmd)
{
	stream << "[" << cmd.angle1 << ", " << cmd.angle2 << ", " << cmd.relPosZ << ", " << cmd.extrudeLength << "]";
	return stream;
}

position operator-(const position &val1, const position &val2)
{
    position retval = {val1.x - val2.x, val1.y - val2.y, val1.z - val2.z};
    return retval;
}


position operator+(const position &val1, const position &val2)
{
    position retval = {val1.x + val2.x, val1.y + val2.y, val1.z + val2.z};
    return retval;
}

/*
position operator+=(const position &val)
{
	this.x += val.x;
	this.y += val.y;
	this.z += val.z;
}*/
