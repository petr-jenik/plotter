#ifndef HW_SERVO_H_
#define HW_SERVO_H_

#include "global.h"

#define MIN_PULSE_WIDTH              1000 //us
#define MAX_PULSE_WIDTH              2000 //us

//timer pulse 840 = 1ms

#define microsecToTimerPulse(X) ((X * 840)/1000)

#define MIN_SERVO_ANGLE             0.0f
#define MAX_SERVO_ANGLE             180.0f //us

#define SERVO_PIN                 GPIO_PIN_7
#define SERVO_PORT                GPIOB

void hwServoInit();
void hwServoSetPosition(float angle, uint32_t channel);

#endif // HW_SERVO_H_
