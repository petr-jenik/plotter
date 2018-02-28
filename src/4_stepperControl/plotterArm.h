/*
 * stepper.h
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#ifndef PLOTTER_ARM_H_
#define PLOTTER_ARM_H_

#include "stepperConfig.h"
#include "calibration.h"

//HW dependent includes
//#include "hwGpio.h"

#include "hwStepperPins.h"

class PlotterArm
{
private:
    // Flags
    bool calibrationEnabled; //TODO maybe rename this flag to hasLimitSwitches
    eState calibrationState;

	const int stepSize = 1;

    bool directionLeft;        // Movement direction
    int setpointStepperValue;  // Desired position (steps)
    int actualStepperValue;    // Actual position (steps)
    int maxStepperValue;       // Max value (steps)
    bool makeStep;             // Get set in updateRegulation - if low, there is no need for moving, if high - move
    bool enableFlag;           // Enable or disable flag

    StepperGPIOs * gpio;

    // HW GPIOs
   /*
    Gpio directionPin;
    Gpio stepPin;
    Gpio resetPin;
    Gpio sleepPin;
    Gpio enablePin;

    Gpio switchPin1;
    Gpio switchPin2;
    */
    const float armAngleOffset;
    const float minAngle;
    const float maxAngle;

    void _enable(void);
    void _disable(void);
    void _reset(void);
    void _sleep(bool state);
    eStepperPosition _getPosition(void);
    bool _isAtLimitPosition(void);
    void _makeCalibrationStep(void);

public:
    PlotterArm(const ArmConfig _armConfig);
    void registerGPIOs(StepperGPIOs * gpio);
    bool Calibrate(void); //Return true if calibration is done

    void OnUpdate(StepperSetting * pSetting);
    void OnUpdateRegulation(void);

    void moveStart(void);
    void moveEnd(void);
    int getError(void);
};

#endif /* PLOTTER_ARM_H_ */
