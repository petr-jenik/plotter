/*
 * movementControl_main.h
 *
 *  Created on: 24. 2. 2018
 *      Author: apollo
 */

#ifndef PLOTTER_3_MOVEMENT_CONTROL_MOVEMENTCONTROL_MAIN_H_
#define PLOTTER_3_MOVEMENT_CONTROL_MOVEMENTCONTROL_MAIN_H_

#include "global.h"

void movementControl_init(void);
void movementControl_createLine(const moveCommand& cmd);
void movementControl_showDemo(void);
void movementControl_drawBorder(void);




#endif /* PLOTTER_3_MOVEMENT_CONTROL_MOVEMENTCONTROL_MAIN_H_ */
