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

#include <cstdlib>

bool timerInitialized = false;

Servo::Servo(int _channel)
:angle(0),
 channel(_channel),
 enableFlag(false)
{
    if (false == timerInitialized)
    {
        servoInit();
    }

    //this->angle = 0;
    //this->channel = (uint32_t)channel;
    //this->enableFlag = false;
}


void Servo::OnUpdate(ServoSetting * pSetting)
{
   if (pSetting != NULL)
   {
       // Enable or disable servo channel
       this->enableFlag = pSetting->enable;
       //this->
       //this->angle = pSetting->value * (MAX_SERVO_ANGLE - MIN_SERVO_ANGLE) / maxSetpointValueInPercent;
   }
}


void Servo::OnMove(void)
{
    servoSetPosition(this->enableFlag * 180, this->channel);
}

