/*
 * global.h
 *
 *  Created on: 18. 2. 2017
 *      Author: apollo
 */

#ifndef PRINTER_GLOBAL_H_
#define PRINTER_GLOBAL_H_

#include <stddef.h>
#include <stdint.h>

#include "log.h"

#ifdef OS_EMBEDDED

#define new_assert(x) if(!(x))                                                                         \
				  {																				    \
						while(1)                                                                    \
						{												                            \
						}                                                                           \
	              }
#endif //OS_EMBEDDED
#ifdef OS_LINUX
threads
#define new_assert(x) if(!(x))                                                                         \
				  {																				    \
						LOG("ASSERT FAILED: ");\
						while(1)                                                                    \
						{												                            \
						}                                                                           \
	              }
#endif //OS_LINUX
//#include <iostream>


void heartbeat(void) __attribute((weak));
void atLimitSwitch(int i) __attribute((weak));

#define MIN(x,y) ((x) < (y)) ? (x) : (y)
#define MAX(x,y) ((x) > (y)) ? (x) : (y)

#define ARRAY_SIZE(x) (sizeof((x))/sizeof((x)[0]))

#define TO_UPPER(x) (((x) >= 'a') && ((x) <= 'z')) ? ((x) + 'A' - 'a') : (x)

typedef struct position
{
    float x;
    float y;
    float z;

    position operator+(const struct position &right)
    {
    	return {this->x + right.x, this->y + right.y, this->z + right.z};
    }

    position operator-(const struct position &right)
    {
    	return {this->x - right.x, this->y - right.y, this->z - right.z};
    }
} position;


typedef struct
{
	float extrudeLength;
	float movementSpeed;
	position finalPosition;
} moveCommand;

typedef struct
{
	float extrudeLength;
	float movementSpeed;
	position startPosition;
	position endPosition;
} guiCommand;

typedef struct
{
	float relativeStepper1;
	float relativeStepper2;
	float relativeStepper3;

	int32_t relPosZ;        // Relative position in Z axe
	float angle1;
	float angle2;
	float extrudeLength;
	float speed;
} armCommand;


template<typename T> T constrain(const T value, const T minValue, const T maxValue)
{
	T result = MIN(value, maxValue);
	result = MAX(result, minValue);
	return result;
}

template<typename T> T map(T x, T in_min, T in_max, T out_min, T out_max)
{
  return (((x - in_min) * (out_max - out_min))/ (in_max - in_min)) + out_min;
}

template<typename T> T template_ABS(T value)
{
	return (value < 0) ? (-value) : (value);
}

/*
int mapInt2(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (((x - in_min) * (out_max - out_min) + (in_max - in_min)/2 )/ (in_max - in_min)) + out_min;
}*/

//position position::operator+(position lhs, const position &rhs)

#endif /* PRINTER_GLOBAL_H_ */
