#pragma once

#define LOG(message)
#define DBG(message)

/*
#include <iostream>
#include <sstream>
#include <thread>

#define LOG(message) {						\
	std::ostringstream os;					\
	os << message << std::endl;				\
	std::cout << os.str();					\
 }

#define LOG(message) {						\
	std::ostringstream os;					\
	os << message << std::endl;				\
	std::cout << os.str();					\
 }

#define DBG(message) {                      \
	std::ostringstream os;                  \
	os << message << std::endl;	            \
	std::cout << os.str();                  \
	}


std::ostream& operator<< (std::ostream& stream, position pos)
{
	stream << "[" << pos.x << "," << pos.y << "," << pos.z << "]";
	return stream;
}


std::ostream& operator << (std::ostream& stream, const moveCommand& cmd)
{
	stream << "Go to: " << cmd.finalPosition;
	stream << ", extrude: " << cmd.extrudeLength;
	stream << ", movementSpeed: " << cmd.movementSpeed;
	return stream;
}


std::ostream& operator << (std::ostream& stream, const armCommand& cmd)
{
	stream << "[" << cmd.angle1 << ", " << cmd.angle2 << ", " << cmd.relPosZ << ", " << cmd.extrudeLength << "]";
	return stream;
}



*/
