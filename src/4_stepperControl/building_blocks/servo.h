/*
 * servp.h
 *
 *  Created on: April 26, 2016
 *      Author: apollo
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "global.h"
#include "stepperConfig.h"

class PlotterServo
{
private:
	float requiredAngle; // Desired angle
	float servoAngle;    // Actual servo angle
    float angleOffset;
    uint32_t channel; // Servo channel
    bool initDone;
    bool enableFlag;     // Enable/disable flag
    float minAngle;
    float maxAngle;
    bool reverseAngle;

public:
    PlotterServo(uint32_t _channel, float _offset, bool reverse, int _minAngle, int _maxAngle);
    void OnUpdate(ServoSetting & setting);
    void OnMove(void);
};

#endif /* SERVO_H_ */
