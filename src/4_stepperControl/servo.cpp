/*
 * servo.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: apollo
 */
#include "servo.h"
#include "hwServo.h"
#include "stepperConfig.h"
#include "config.h"

#include <stdlib.h>

bool timerInitialized = false;

Servo::Servo(int channel)
{
    if (false == timerInitialized)
    {
        servoInit();
    }

    this->angle = 0;
    this->channel = (uint32_t)channel;
    this->enableFlag = false;
}


void Servo::OnUpdate(ServoSetting * pSetting)
{
   if (pSetting != NULL)
   {
       // Enable or disable servo channel
       this->enableFlag = pSetting->enable;
       //this->angle = pSetting->value * (MAX_SERVO_ANGLE - MIN_SERVO_ANGLE) / maxSetpointValueInPercent;
   }
}


void Servo::OnMove(void)
{
    servoSetPosition(this->enableFlag, this->channel);
}

