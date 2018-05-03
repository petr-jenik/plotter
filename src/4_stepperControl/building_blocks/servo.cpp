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
#include "global.h"
#include <cstdlib>

PlotterServo::PlotterServo(uint32_t _channel, float _offset, bool reverse, int _minAngle, int _maxAngle)
:requiredAngle(0),
 servoAngle(0),
 angleOffset(_offset),
 channel(_channel),
 initDone(false),
 enableFlag(false),
 minAngle(_minAngle),
 maxAngle(_maxAngle),
 reverseAngle(reverse)
{
}


void PlotterServo::OnUpdate(ServoSetting & setting)
{
   if (initDone == false)
   {
	   hwServoInit(this->channel);
	   initDone = true;
   }

   // Enable or disable servo channel
   this->enableFlag = setting.enable;
   //this->requiredAngle = setting.angle;//setting.angle + this->offset;//templateConstrain(setting.angle, MIN_SERVO_ANGLE, MAX_SERVO_ANGLE);

   this->requiredAngle = templateConstrain(setting.angle, this->minAngle, this->maxAngle);

	if (reverseAngle == true)
	{
		this->servoAngle = this->angleOffset - requiredAngle;
	}
	else
	{
		this->servoAngle = this->angleOffset + requiredAngle;
	}
};


void PlotterServo::OnMove(void)
{
	hwServoSetPosition(this->servoAngle, this->channel);
}

