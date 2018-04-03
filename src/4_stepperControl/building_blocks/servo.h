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
	// Desired angle
    float angle;

	// Servo channel
    uint32_t channel;

    bool initDone;

    // Enable/disable flag
    bool enableFlag;

public:
    PlotterServo(int channel);
    void OnUpdate(ServoSetting & setting);
    void OnMove(void);
};

#endif /* SERVO_H_ */
