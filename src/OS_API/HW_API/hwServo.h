#ifndef HW_SERVO_H_
#define HW_SERVO_H_

//#include "global.h"
#include <cstdint>

#define MIN_SERVO_ANGLE             0.0f
#define MAX_SERVO_ANGLE             180.0f //us

void servoInit(int32_t channel);
void servoSetPosition(float angle, uint32_t channel);

#endif // HW_SERVO_H_
