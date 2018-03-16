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

