/*
 * mainApp.h
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#ifndef APP_MAINAPP_H_
#define APP_MAINAPP_H_

#include "global.h"

void stepperControl_init();

void stepperControl_goToThisPosition(position newPosition,float extrudeLength);

bool isSystemReady(void);

#endif /* APP_MAINAPP_H_ */
