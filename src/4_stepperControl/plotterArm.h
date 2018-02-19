/*
 * stepper.h
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include "myGlobal.h"

#include "hwStepperPins.h"
#include "stepperConfig.h"

#include "calibration.h"

//HW dependent includes
#include "hwGpio.h"

using namespace std;

class PlotterArm
{
//private:
public:
	const int stepSize = 1;

    //Stepper setting
	//StepperSetting stepperSetting;

    // Flags
    bool calibrationEnabled; //TODO maybe rename this flag to hasLimitSwitches
    eState calibrationState;

    const float armAngleOffset;
    const float minAngle;
    const float maxAngle;

    bool directionLeft;        // Movement direction
    //uint32_t setpointValueInPercent; // Desired position in percents - (thousandths of a percent)
    int setpointStepperValue;  // Desired position (steps)
    int actualStepperValue;    // Actual position (steps)
    int maxStepperValue;       // Max value (steps)
    bool makeStep;             // Get set in updateRegulation - if low, there is no need for moving, if high - move
    bool enableFlag;           // Enable or disable flag

    // HW GPIOs
    Gpio directionPin;
    Gpio stepPin;
    Gpio resetPin;
    Gpio sleepPin;
    Gpio enablePin;

    Gpio switchPin1;
    Gpio switchPin2;

public://StepperController
    PlotterArm(sStepperPins pinsDescription, const ArmConfig _armConfig, sSwichPins switchPins = {undefPin, undefPin});
    //Stepper(IEventHa//ndler &handler, sStepperPins gpios);

    bool Calibrate(void); //Return true if calibration is done
    //void OnUpdate(void);
    void OnUpdate(StepperSetting * pSetting);
    void OnUpdateRegulation(void);

    void moveStart(void);
    void moveEnd(void);
    int getError(void);

private:
    void _enable(void);
    void _disable(void);
    void _reset(void);
    void _sleep(bool state);
    eStepperPosition _getPosition(void);
    bool _isAtLimitPosition(void);
    void _makeCalibrationStep(void);
};

#endif /* STEPPER_H_ */
