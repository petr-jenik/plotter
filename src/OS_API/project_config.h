/*
 * config.h
 *
 *  Created on: 26. 2. 2017
 *      Author: apollo
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "global.h"

const int SPEED_MAGICAL_CONSTANT = 50;

const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;


const int LEFT_ARM_OFFSET = +45;
const int RIGHT_ARM_OFFSET = -45;

const int STEP_SIZE = 1;

const position pos_S1 = {20, -100, 0};
const position pos_S2 = {-20, -100, 0};

const float zAxeMin = 0;
const float zAxeMax = 1000;
const float maxRelativeZ = 100000.0;

const float cMinimalDistanceToCenter = 1;

const int armLength_AC =  100;
const int armLength_AS1 = 100;
const int armLength_BC =  100;
const int armLength_BS2 = 100;

//TODO Move this to a better location
const int armLength_AD =  20;
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