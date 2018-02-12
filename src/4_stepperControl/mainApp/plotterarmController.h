/*
 * stepperController.h
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#ifndef STEPPERCONTROLLER_H_
#define STEPPERCONTROLLER_H_

//#include "IDatabase.h"
#include <plotterarm.h>
#include "stepperConfig.h"
#include "servo.h"
#include "global.h"


typedef enum
{
    eObjectType_stepper = 1,
    eObjectType_servo,

    eObjectType_COUNT
} eObjectType;

class ArmController
{
private:
	bool bCalibrationFinished;

    PlotterArm & armLeft;
    PlotterArm & armRight;
    //Stepper stepper_z_axis;
    Servo & servo;

    PlotterArm * armObjectArray[10];
    int armObjectCount;

    Servo * servoObjectArray[10];
    int servoObjectCount;

    int GetMaxStepperError(void);
public:
    ArmController(PlotterArm & _arm_left, PlotterArm & _arm_right, Servo & _servo);
    void OnUpdateAll(armCommand command);
    void OnMove(void);
    void calibrate(bool startNewCalibration);
    bool calibrationFinished();
};

#endif /* STEPPERCONTROLLER_H_ */
