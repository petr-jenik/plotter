/*
 * building_blocks.cpp
 *
 *  Created on: 2 Mar 2018
 *      Author: pi
 */

#include "building_blocks.h"
#include "calibration.h"
#include "project_config.h"
#include "global.h"
#include "hwStepperPins.h"
#include "math_tools.h"
#include "stepperConfig.h"
#include "Timer.h"

Stepper::Stepper()
	:gpio(NULL),
	 error(0),
	 sumError(0),
	 currentStepCount(0),
     directionLeft(false),
     makeStep(false),
     enableFlag(false)
{}


void Stepper::registerGPIOs(StepperGPIOs * gpio)
{
	this->gpio = gpio;

    // Initialize all GPIOs
    this->gpio->enablePin.powerUp();
    this->gpio->stepPin.powerUp();
    this->gpio->directionPin.powerUp();
    this->gpio->resetPin.powerUp();
    this->gpio->sleepPin.powerUp();

    //TODO Disable stepper
	//Enable stepper
    this->_sleep(false);
    this->_reset();
    this->_enable();
}

void Stepper::OnUpdate(int32_t numberOfSteps, bool enable)
{
	//this->sumError = 0;
	this->sumError += numberOfSteps;
	DBG(__FUNCTION__ << " current step:" << this->sumError);
	this->error = numberOfSteps;

    // Enable or disable stepper controller - enable only once
    if (this->enableFlag != enable)
    {
        if (true == enable)
        {
            this->_enable();
        }
        else
        {
            this->_disable();
        }
        this->enableFlag = enable;
    }
}


int32_t Stepper::getError(void)
{
	return this->error;
}

void Stepper::OnUpdateRegulation(void)
{
	// Send heartbeat signal
	heartbeat();

    if (false == this->enableFlag)
    {
        this->makeStep = false;
        return;
    }

    if (this->error == 0)
    {
        this->makeStep = false;
        return;
    }

    if (this->error > 0)
    {
        this->directionLeft = false;
    }
    else
    {
        this->directionLeft = true;
    }

    this->makeStep = true;
}

void Stepper::moveStart(void)
{
    if (true == this->makeStep)
    {
        if (true == this->directionLeft)
        {
        	this->currentStepCount -= this->stepSize;
            this->error += this->stepSize;
            this->gpio->directionPin.turnOn();
        }
        else
        {
        	this->currentStepCount += this->stepSize;
        	this->error -= this->stepSize;
            this->gpio->directionPin.turnOff();
        }
        this->gpio->stepPin.turnOn();
    }
}

void Stepper::moveEnd(void)
{
    if (true == this->makeStep)
    {
        this->gpio->stepPin.turnOff();
    }
    DBG("current step count: " << this->currentStepCount);
    DBG("current error: " << this->error);
}

void Stepper::forceMove(int32_t relativeStepCount)
{
	DBG("relativeStepCount: " << relativeStepCount);
	this->OnUpdate(relativeStepCount, true);
	while(this->getError() != 0 )
	{
		this->OnUpdateRegulation();
		this->moveStart();
		this->moveEnd();
	}
}

void Stepper::_enable(void)
{
    this->gpio->enablePin.turnOn();
}

void Stepper::_disable(void)
{
    this->gpio->enablePin.turnOff();
}

void Stepper::_reset(void)
{
    this->gpio->resetPin.turnOn();

    // wait for a configured time period
    Timer::sleep(getTimeDelayConfig().resetPulseLength);

    this->gpio->resetPin.turnOff();
}

void Stepper::_sleep(bool state)
{
    if (true == state)
    {
        //Enable sleep mode
        this->gpio->sleepPin.turnOn();
    }
    else
    {
        //Disable sleep mode
        this->gpio->sleepPin.turnOff();
    }
}


///////// StepperWithLimits

StepperWithLimits::StepperWithLimits()
:limitSwitchGPIOs(NULL),
 maxStepperValue(0),
 calibrationEnabled(false),
 calibrationState(eState_Start)
{
}


void StepperWithLimits::registerLimitSwitchGPIOs(LimitSwitchGPIOs * gpio)
{
	this->limitSwitchGPIOs = gpio;

	// Initialize all GPIOs
	this->limitSwitchGPIOs->switchPin1.powerUp();
	this->limitSwitchGPIOs->switchPin2.powerUp();
}

eStepperPosition StepperWithLimits::getPosition(void)
{
    eStepperPosition position = eStepperPosition_Undef;

    DBG("SW1: " << this->limitSwitchGPIOs->switchPin1.isOn() << ", SW2:" << this->limitSwitchGPIOs->switchPin2.isOn());

    bool status = this->limitSwitchGPIOs->switchPin1.isOn();
    status |= this->limitSwitchGPIOs->switchPin2.isOn();

    if (status == true)
    {
        position = eStepperPosition_AtLimitSwitch;
    }
    return position;
}

void StepperWithLimits::OnUpdate(float relativePosition, bool enable)
{
	int32_t setpointStepperValue = templateMap(relativePosition,
                                               (float)0.0,
												 (float)1.0,
												 (float)0.0,
												 (float)this->maxStepperValue); //TODO Proc je max stepper value o 1 vetsi nez by mela byt?

	//setpointStepperValue = templateConstrain(setpointStepperValue, minStepperValue, this->maxStepperValue);

	int32_t diff = setpointStepperValue - this->currentStepCount;

	if (this->getPosition() == eStepperPosition_AtLimitSwitch)
	{
		if (diff < 0)
		{
			this->getPosition();
			// New direction will be "to the left"
			this->currentStepCount = 0;
		}
		else
		{
			// Nothing
			// Let stepper go right
			//this->currentStepCount = this->maxStepperValue;
		}
	}

	int32_t minStepperValue = 0;
	setpointStepperValue = templateConstrain(setpointStepperValue, minStepperValue, this->maxStepperValue);
	diff = setpointStepperValue - this->currentStepCount;

	Stepper::OnUpdate(diff, enable);
}

bool StepperWithLimits::Calibrate(void)
{
    eStepperCmd command;

    this->calibrationState = transitionFunction(this->calibrationState,
                                                this->getPosition(),
                                                &command);

    DBG("Calibration state:" <<  calibrationState);

    switch(calibrationState)
    {
        case eState_Left:
        	atLimitSwitch(0);
        	this->currentStepCount = 0;
            break;
        case eState_Right:
        	atLimitSwitch(1);
            this->maxStepperValue = this->currentStepCount;
            LOG("max stepper value" << this->maxStepperValue);

            break;
        default:
            break;
    }

    switch(command)
    {
    case eStepperCmd_GoToLeft:
    	Stepper::OnUpdate(-this->stepSize, true);
    	Stepper::OnUpdateRegulation();
        break;

    case eStepperCmd_GoToRight:
    	Stepper::OnUpdate(this->stepSize, true);
    	Stepper::OnUpdateRegulation();
    	break;

    case eStepperCmd_Stop:
    	Stepper::OnUpdate(0, true);
    	Stepper::OnUpdateRegulation();
        break;

    default:
    	Stepper::OnUpdate(0, true);
    	Stepper::OnUpdateRegulation();
    	break;
    }

    return (eState_Done == this->calibrationState);
}


/////////// StepperWithOneLimitSwitch


StepperWithOneLimitSwitch::StepperWithOneLimitSwitch(int32_t maxStepperValue)
	: limitSwitch(NULL)
{
	this->maxStepperValue = maxStepperValue;
}

void StepperWithOneLimitSwitch::registerSingleLimitSwitchGPIO(Gpio *gpio)
{
	this->limitSwitch = gpio;

	// Initialize the GPIO
	this->limitSwitch->powerUp();
}

eStepperPosition StepperWithOneLimitSwitch::getPosition(void)
{
    eStepperPosition position = eStepperPosition_Undef;

    bool isOn = false;
    bool isOn_old = isOn;

    const int cMaxNumberOfAttempts = 10;
    int numberOfAttempts = cMaxNumberOfAttempts;

    // A result of reading of GPIO state has to be the same at least for 5 consecutive attempts
    while (numberOfAttempts > 0)
    {
    	isOn = this->limitSwitch->isOn();
    	if (isOn == isOn_old)
    	{
    		numberOfAttempts--;
    	}
    	else
    	{
    		numberOfAttempts = cMaxNumberOfAttempts;
    	}
    	isOn_old = isOn;
    }

    //LOG("SW1: " << isOn);

    if (isOn)
    {
        position = eStepperPosition_AtLimitSwitch;
    }
    else
    {
    	position = eStepperPosition_Undef;
    }
    return position;
}

bool StepperWithOneLimitSwitch::Calibrate(void)
{
    bool calibrationFinished = false;
    eStepperPosition position = this->getPosition();

    if (position == eStepperPosition_AtLimitSwitch)
    {
    	// Calibration is done
    	atLimitSwitch(0);
    	calibrationFinished = true;
    	this->currentStepCount = 0;

    	// Stop stepper
    	Stepper::OnUpdate(0, true);
		Stepper::OnUpdateRegulation();
    }
    else
    {
    	// Go to the left and hit the limit switch
		Stepper::OnUpdate(-this->stepSize, true);
		Stepper::OnUpdateRegulation();
		calibrationFinished = false;
    }
    return calibrationFinished;
}

/////////// PlotterArm



PlotterArm::PlotterArm(const PlotterArmConfig _armConfig)
	:minAngle(_armConfig.angleMin),
	maxAngle(_armConfig.angleMax),
	armAngleOffset(_armConfig.angleOffset)
{}


void PlotterArm::NewPosition(PlotterArmSetting &setting)
{
	float requiredAngle = toPositiveAngle(setting.setpointAngle - this->armAngleOffset);
	requiredAngle = templateConstrain(requiredAngle, this->minAngle, this->maxAngle);

	float relativePosition = templateMap(requiredAngle,
                                  (float)this->minAngle,
                                  (float)this->maxAngle,
                                  (float)0.0,
                                  (float)1.0);
	StepperWithLimits::OnUpdate(relativePosition, setting.enable);
}


