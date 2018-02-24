/*
 * calibration.h
 *
 *  Created on: Jan 19, 2016
 *      Author: apollo
 */

#ifndef APP_CALIBRATION_H_
#define APP_CALIBRATION_H_

typedef enum
{
    eState_Start = 0,
    eState_Left,
    eState_Undef,
    eState_Right,
    eState_Done,
} eState;

typedef enum
{
    eStepperCmd_GoToLeft = 0,
    eStepperCmd_GoToRight,
    eStepperCmd_Stop,
} eStepperCmd;

typedef enum
{
    eStepperPosition_Undef = 0,
    eStepperPosition_AtLimitSwitch,
} eStepperPosition;

//transition table
struct transitionResponse
{
    eState newState;
    eStepperCmd output;
};

struct transitionItem
{
    //eState currentState;
    struct transitionResponse outputFor0;
    struct transitionResponse outputFor1;
};

eState transitionFunction(eState state, eStepperPosition position, eStepperCmd * output);

#endif /* APP_CALIBRATION_H_ */
