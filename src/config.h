/*
 * config.h
 *
 *  Created on: 26. 2. 2017
 *      Author: apollo
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "global.h"

int32_t angleToRelative(float angle);
float relativeToAngle(int32_t relative);

const int armLength_AC = 200;
const int armLength_AS1 = 200;
const int armLength_BC = 200;
const int armLength_BS2 = 200;

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

extern position pos_S1;
extern position pos_S2;

#endif /* _CONFIG_H_ */
