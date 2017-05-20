/*
 * global.h
 *
 *  Created on: 18. 2. 2017
 *      Author: apollo
 */

#ifndef PRINTER_GLOBAL_H_
#define PRINTER_GLOBAL_H_

#include <stdint.h>
#include <iostream>

#include <iostream>
#include <sstream>
#include <thread>

#define LOG(message) {						\
	std::ostringstream os;					\
	os << std::this_thread::get_id() << " ";		\
	os << message << std::endl;				\
	std::cout << os.str();					\
 }

#define DBG(message) {                      \
	std::ostringstream os;                  \
	os << std::this_thread::get_id() << " ";\
	os << message << std::endl;	            \
	std::cout << os.str();                  \
	}

typedef struct
{
    float x;
    float y;
    float z;
} position;


typedef enum
{
	eLine,
	eMove
} moveCommandType;


typedef struct
{
	bool extrude;
	float movementSpeed;
	//moveCommandType type;
	position pos1;
	position pos2;
} moveCommand;

typedef struct
{
	int32_t stepper1;
	int32_t stepper2;
	int32_t stepper3;
} stepperCommand;

typedef struct
{
	int32_t relativeAngle1; // Position of arm in 100.000 %
	int32_t relativeAngle2; // Position of arm in 100.000 %
	int32_t relPosZ;        // Relative position in Z axe
	bool extrude;
} armCommand;


std::ostream& operator<< (std::ostream& stream, position pos);
std::ostream& operator << (std::ostream& stream, const stepperCommand& cmd);
std::ostream& operator << (std::ostream& stream, const moveCommand& cmd);
std::ostream& operator << (std::ostream& stream, const armCommand& cmd);
#endif /* PRINTER_GLOBAL_H_ */
