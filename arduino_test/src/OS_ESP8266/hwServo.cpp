/*
 * servo.c
 *
 *  Created on: Mar 12, 2016
 *      Author: apollo
 */

#include "hwServo.h"
#include "global.h"
#include <Servo.h>

//#define ARRAY_SIZE(X) (sizeof((X)) / sizeof((X[0])))

int gpios[] = {D0, D1, D2};
const int cServoCount = ARRAY_SIZE(gpios);

Servo servos[cServoCount];  // create servo object to control a servo
                // twelve servo objects can be created on most boards

void hwServoInit(int32_t channel, float defaultAngle)
{
	TRACE; // Trace macro

	if (channel < cServoCount)
	{
		uint8_t assignedChannel = servos[channel].attach(gpios[channel]);  // attaches the servo on GIO2 to the servo object
		DBG("assignedChannel: " << assignedChannel);
    }
	else
	{
		DBG("Invalid servo channel: " << channel);
	}

	// Go to default angle
	hwServoSetPosition(defaultAngle, channel);

}


void hwServoSetPosition(float angle, uint32_t channel)
{
	TRACE; // Trace macro

	if ((int)channel < cServoCount)
	{
		// Calculate pulse width in microseconds
		float pulseWidth = templateMap(angle, 0.0f, 180.0f, (float)MIN_PULSE_WIDTH, (float)MAX_PULSE_WIDTH);

        servos[channel].writeMicroseconds((int) pulseWidth);
		DBG("channel: " << channel << ", angle: " << (int) angle << ", read back angle: " << servos[channel].read());
        //delay(15);  // waits 15ms for the servo to reach the position
	}
}


