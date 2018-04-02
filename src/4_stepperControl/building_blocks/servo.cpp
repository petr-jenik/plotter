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
        hwServoInit();
    }

    //this->angle = 0;
    //this->channel = (uint32_t)channel;
    //this->enableFlag = false;
}


void Servo::OnUpdate(ServoSetting & setting)
{
   // Enable or disable servo channel
   this->enableFlag = setting.enable;
   this->angle = constrain(setting.angle, MIN_SERVO_ANGLE, MAX_SERVO_ANGLE);
}


void Servo::OnMove(void)
{
    hwServoSetPosition(this->angle, this->channel);
}

