/*
 * servo.c
 *
 *  Created on: Mar 12, 2016
 *      Author: apollo
 */

#include "hwTimer.h"
#include "hwServo.h"
#include "global.h"

#define MIN_PULSE_WIDTH       612 // 300 //544     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH       2280//2500 //2700 //2400     // the longest pulse sent to a servo
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached

void hwServoInit(int32_t channel)
{
    TIM_Timer4_Init();
    InitializePWM();
}

#define microsecToTimerPulse(X) ((X * 840)/1000)

void hwServoSetPosition(float angle, uint32_t channel)
{
    uint32_t TIM_channel = 0;
    switch (channel)
    {
    case 1:
        TIM_channel = TIM_CHANNEL_1;
        break;

    case 2:
        TIM_channel = TIM_CHANNEL_2;
        break;

    case 3:
        TIM_channel = TIM_CHANNEL_3;
        break;

    case 4:
        TIM_channel = TIM_CHANNEL_4;
        break;

    default:
        return;
    }

	angle = templateConstrain(angle, 0.0f, 180.0f);
	float pulseWidthUs = templateMap(angle, 0.0f, 180.0f, (float)MIN_PULSE_WIDTH, (float)MAX_PULSE_WIDTH);

	uint32_t timerPulse = microsecToTimerPulse(pulseWidthUs);
    updatePulseWidth(eTimer4, timerPulse, TIM_channel);
}
