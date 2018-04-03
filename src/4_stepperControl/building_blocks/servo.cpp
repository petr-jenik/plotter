/*
 * servo.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: apollo
 */
#include "servo.h"
#include "hwServo.h"
#include "stepperConfig.h"
#include "project_config.h"

#include <cstdlib>

PlotterServo::PlotterServo(int32_t _channel)
:angle(0),
 channel(_channel),
 initDone(false),
 enableFlag(false)
{
}


void PlotterServo::OnUpdate(ServoSetting & setting)
{
   if (initDone == false)
   {
	   servoInit(this->channel);
	   initDone = true;
   }

   // Enable or disable servo channel
   this->enableFlag = setting.enable;
   this->angle = constrain(setting.angle, MIN_SERVO_ANGLE, MAX_SERVO_ANGLE);
};


void PlotterServo::OnMove(void)
{
    servoSetPosition(this->angle, this->channel);
}

