/*
 * config.h
 *
 *  Created on: 26. 2. 2017
 *      Author: apollo
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "global.h"

//#define FLIP_IMAGE_ALONG_X_AXIS 1

#define PRINTER_TYPE_2D_PLOTTER_WITH_SERVOS            1
#define PRINTER_TYPE_2D_PLOTTER_TYPE_SCARA_ROBOT       2
#define PRINTER_TYPE_2D_PLOTTER_CARTESIAN              3
#define PRINTER_TYPE_3D_PRINTER_TYPE_SCARA             4
#define PRINTER_TYPE_3D_PRINTER_CARTESIAN              5

// Select type of connected hardware
#define PRINTER_TYPE PRINTER_TYPE_2D_PLOTTER_CARTESIAN

const int SPEED_MAGICAL_CONSTANT = 5;

const int MIN_ANGLE = -45;
const int MAX_ANGLE = 180;


const int LEFT_ARM_OFFSET = 140;
const int RIGHT_ARM_OFFSET = 61;

const int STEP_SIZE = 1;

#if PRINTER_TYPE == PRINTER_TYPE_2D_PLOTTER_WITH_SERVOS

/*              D
 *             /
 *           C/
 *           /\
 *          /  \
 *         /    \
 *        /      \
 *       A        B
 *        \      /
 *         \    /
 *          \  /
 *  right - S1 S2 - left
 *
 */
	const position pos_S1 = {25, -120, 0};
	const position pos_S2 = {-25, -120, 0};

	const int armLength_AC =  85;
	const int armLength_AS1 = 85;
	const int armLength_BC =  85;
	const int armLength_BD = 100;
	const int armLength_BS2 = 85;
	const int armLength_CD =  20;

#elif PRINTER_TYPE == PRINTER_TYPE_2D_PLOTTER_TYPE_SCARA_ROBOT

	/*
	 *           C
	 *           /\
	 *          /  \
	 *         /    \
	 *        /      \
	 *       A        B
	 *        \      /
	 *         \    /
	 *          \  /
	 *          S1 S2
	 *
	 */

	const position pos_S1 = {0, -100, 0};
	const position pos_S2 = {0, -100, 0};

	const int armLength_AC =  100;
	const int armLength_AS1 = 100;
	const int armLength_BC =  100;
	const int armLength_BS2 = 100;

#elif PRINTER_TYPE == PRINTER_TYPE_2D_PLOTTER_CARTESIAN

	const position pos_START = { -25, -25, 0}; // Bottom left corner
	const position pos_END   = { 25, 25, 0}; // Upper right corner

	enum
	{
		eStepperIdx_X = 0,
		eStepperIdx_Y,

		eStepperIdx_COUNT
	} eStepperIdx2D;


	const int STEPPER_X_NUMBER_OF_STEPS = 1000;
	const int STEPPER_Y_NUMBER_OF_STEPS = 1000;

	const int SERVO_ANGLE_MIN = 50;
	const int SERVO_ANGLE_MAX = 65;
	const int SERVO_CHANNEL = 1;
	const int SERVO_OFFSET = 180;

#elif PRINTER_TYPE == PRINTER_TYPE_3D_PRINTER_CARTESIAN

	const position pos_START = { -150, -150, -150}; // Bottom left corner
	const position pos_END = { 150, 150, 150}; // Upper right corner

	enum
	{
		eStepperIdx_X = 0,
		eStepperIdx_Y,
		eStepperIdx_Z,

		eStepperIdx_COUNT
	} eStepperIdx3D;

#endif

const float zAxeMin = 0;
const float zAxeMax = 1000;
const float maxRelativeZ = 100000.0;

const float cMinimalDistanceToCenter = 1;


#endif /* _CONFIG_H_ */
