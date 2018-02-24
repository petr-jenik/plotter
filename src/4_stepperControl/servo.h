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
    // Flags
    uint32_t angle;                 // Desired angle
    bool enableFlag;                // Enable or disable flag

    // Servo channel
    uint32_t channel;

public:
    Servo(int channel);
    void OnUpdate(ServoSetting * pSetting);
    void OnMove(void);
};

#endif /* SERVO_H_ */
