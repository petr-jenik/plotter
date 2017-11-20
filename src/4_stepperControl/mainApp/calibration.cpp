/*
 * calibration.cpp
 *
 *  Created on: Jan 19, 2016
 *      Author: apollo
 */

#include "calibration.h"

struct transitionItem transitionTable[] =
{
    {{eState_Start, eStepperCmd_GoToLeft},  {eState_Left, eStepperCmd_Stop}},      //eState_Start,
    {{eState_Undef, eStepperCmd_GoToRight}, {eState_Left, eStepperCmd_GoToRight}}, //eState_Left,
    {{eState_Undef, eStepperCmd_GoToRight}, {eState_Right, eStepperCmd_Stop}},     //eState_Undef,
    {{eState_Done, eStepperCmd_Stop},       {eState_Right, eStepperCmd_GoToLeft}}, //eState_Right,
    {{eState_Done, eStepperCmd_Stop},       {eState_Done, eStepperCmd_Stop}},      //eState_Done,
};

eState transitionFunction(eState state, eStepperPosition position, eStepperCmd *output)
{
    state = (eState) state;

    eState newState;

    if (position == eStepperPosition_Undef)
    {
        newState = transitionTable[state].outputFor0.newState;
        *output   = transitionTable[state].outputFor0.output;
    }
    else
    {
        newState = transitionTable[state].outputFor1.newState;
        *output   = transitionTable[state].outputFor1.output;
    }

    return newState;
}
