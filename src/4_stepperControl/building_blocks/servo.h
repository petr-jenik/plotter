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

class Servo
{
private:
	// Desired angle
    float angle;

	// Servo channel
    uint32_t channel;

    // Enable/disable flag
    bool enableFlag;

public:
    Servo(int channel);
    void OnUpdate(ServoSetting & setting);
    void OnMove(void);
};

#endif /* SERVO_H_ */
