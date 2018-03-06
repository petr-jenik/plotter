/*
 * stepperController.h
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#ifndef STEPPERCONTROLLER_H_
#define STEPPERCONTROLLER_H_

#include "building_blocks.h"
#include "servo.h"
#include "global.h"


typedef enum
{
    eObjectType_stepper = 1,
    eObjectType_servo,

    eObjectType_COUNT
} eObjectType;

const int cMaxNumberOfPlotterArms = 10;
const int cMaxNumberOfSteppers = 10;
const int cMaxNumberOfServos = 10;
const int cNumberOfSteppersWithLimit = 10;

class MechanicController
{
private:
	bool bCalibrationFinished;

    PlotterArm * armObjectArray[cMaxNumberOfPlotterArms];
    int armObjectCount;

    Stepper * stepperObjectArray[cMaxNumberOfSteppers];
    int stepperObjectCount;

    StepperWithLimits *limStepperObjectArray[cNumberOfSteppersWithLimit];
	int limStepperObjectCount;

    Servo * servoObjectArray[cMaxNumberOfServos];
    int servoObjectCount;

    int GetMaxStepperError(void);

    void updateRegulationOnAll(void);
    void moveWithEach(void);

public:
    MechanicController();

    void registerArms(PlotterArm * pArms, int armsCount);
    void registerServos(Servo * pServos, int servosCount);
    void registerSteppers(Stepper * pSteppers, int steppersCount);
    //PlotterArm & _arm_left, PlotterArm & _arm_right, Servo & _servo
    void OnUpdateAll(armCommand command);
    void OnMove(void);
    void calibrate(bool startNewCalibration);
    bool calibrationFinished(void);
};

#endif /* STEPPERCONTROLLER_H_ */