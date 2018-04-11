/*
 * building_blocks.h
 *
 *  Created on: 2 Mar 2018
 *      Author: pi
 */

#ifndef BB_BUILDING_BLOCKS_H_
#define BB_BUILDING_BLOCKS_H_


#include "stepperConfig.h"
#include "calibration.h"
#include "hwStepperPins.h"
#include <stdint.h>

class Stepper
{
protected:
    bool directionLeft;        // Movement direction
    bool makeStep;             // Get set in updateRegulation - if low, there is no need for moving, if high - move
    bool enableFlag;           // Enable or disable flag

    // HW GPIOs for stepper
    StepperGPIOs * gpio;

	int32_t error;
	int32_t sumError;
	int32_t currentStepCount; 	// Actual position (steps)

	const int32_t stepSize = 1;

	void _enable(void);
    void _disable(void);
    void _reset(void);
    void _sleep(bool state);

public:
    Stepper();
    void registerGPIOs(StepperGPIOs * gpio);

    void OnUpdate(int32_t numberOfSteps, bool enable);
    void OnUpdateRegulation(void);

    void moveStart(void);
    void moveEnd(void);
    int32_t getError(void);
};


class StepperWithLimits : public Stepper
{
protected:
   // HW GPIOs for limit switches
    LimitSwitchGPIOs * limitSwitchGPIOs;

    int32_t setpointStepperValue;  // Desired position (steps)
    int32_t maxStepperValue;       // Max value (steps)

    // Flags
    bool calibrationEnabled;
    eState calibrationState;

public:
    StepperWithLimits();
    bool Calibrate(void); //Return true if calibration is done
    eStepperPosition getPosition(void);// <- musi vracet undef, pokud stepper nema limit switche - nebo budou limit scwitche soucasti podedenych trid?
    void OnUpdate(float relativePosition, bool enable);
    void registerLimitSwitchGPIOs(LimitSwitchGPIOs * gpio);
};

class PlotterArm : public StepperWithLimits
{
protected:
    const float minAngle;
    const float maxAngle;
    const float armAngleOffset;

public:
    PlotterArm(const PlotterArmConfig _armConfig);
    void NewPosition(PlotterArmSetting &setting);
};


#endif /* BB_BUILDING_BLOCKS_H_ */
