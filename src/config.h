/*
 * config.h
 *
 *  Created on: 26. 2. 2017
 *      Author: apollo
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "global.h"

enum {eStepperMaxCount = 4};

const int SPEED_MAGICAL_CONSTANT = 10;

const int left_stepper_offset = -45;
const int right_stepper_offset = +45;

const int cStepSize = 1;

const int maxSetpointValueInPercent = 100000; // 100.000% (thousandths of a percent)

const position pos_S1 = {0, -100, 0};
const position pos_S2 = {0, -100, 0};

const float maxRelativeAngle = maxSetpointValueInPercent;

const float zAxeMin = 0;
const float zAxeMax = 1000;
const float maxRelativeZ = 100000.0;

const float cMinimalDistanceToCenter = 1;

const int armLength_AC =  100;
const int armLength_AS1 = 100;
const int armLength_BC =  100;
const int armLength_BS2 = 100;

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
