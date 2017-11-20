/*
 * config.cpp
 *
 *  Created on: 27. 2. 2017
 *      Author: apollo
 */



#include "config.h"

#include "global.h"
#include "stepperConfig.h"


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
	stream << "[" << cmd.angle1 << ", " << cmd.angle2 << ", " << cmd.relPosZ << ", " << cmd.extrude << "]";
	return stream;
}

