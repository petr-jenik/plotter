/*
 * config.h
 *
 *  Created on: 26. 2. 2017
 *      Author: apollo
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "global.h"

#define PRINTER_TYPE_2D_PLOTTER       1
#define PRINTER_TYPE_PLOTTER_CLOCK    2
#define PRINTER_TYPE_3D_PRINTER       3

// Select type of connected hardware
#define PRINTER_TYPE PRINTER_TYPE_PLOTTER_CLOCK

const int SPEED_MAGICAL_CONSTANT = 50;

const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;


const int LEFT_ARM_OFFSET = +45;
const int RIGHT_ARM_OFFSET = -45;

const int STEP_SIZE = 1;

#if PRINTER_TYPE == PRINTER_TYPE_PLOTTER_CLOCK
	const position pos_S1 = {20, -120, 0};
	const position pos_S2 = {-20, -120, 0};
#elif PRINTER_TYPE == PRINTER_TYPE_2D_PLOTTER
	const position pos_S1 = {0, -100, 0};
	const position pos_S2 = {0, -100, 0};
#elif PRINTER_TYPE == PRINTER_TYPE_3D_PRINTER

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

const int armLength_AC =  100;
const int armLength_AS1 = 100;
const int armLength_BC =  100;
const int armLength_BS2 = 100;

//TODO Move this to a better location
const int armLength_CD =  20;
const int armLength_BD = 102;

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

//extern position pos_S1;
//extern position pos_S2;

#endif /* _CONFIG_H_ */
