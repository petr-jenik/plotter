/*
 * building_blocks.cpp
 *
 *  Created on: 2 Mar 2018
 *      Author: pi
 */

#include "assert.h"
#include "building_blocks.h"
#include "calibration.h"
#include "config.h"
#include "global.h"
#include "hwStepperPins.h"
#include "math_tools.h"
#include "stepperConfig.h"
#include "Timer.h"

using namespace std;

Stepper::Stepper()
	:gpio(NULL),
	 error(0),
	 sumError(0),
	 currentStepCount(0)
{
    this->directionLeft = false;
    this->makeStep = false;
    this->enableFlag = false;
}


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

void Stepper::OnUpdate(int numberOfSteps, bool enable)
{
	//this->sumError = 0;
	this->sumError += numberOfSteps;
	//LOG(__FUNCTION__ << " current step:" << this->sumError);
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


int Stepper::getError(void)
{
	return this->error;
}

void Stepper::OnUpdateRegulation(void)
{
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
    LOG("current step count: " << this->currentStepCount);
    LOG("current error: " << this->error);
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

    //TODO Move all delay times to named constants
    Timer::sleep(5);

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
 setpointStepperValue(0),
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

    //LOG("SW1: " << this->limitSwitchGPIOs->switchPin1.isOn() << ", SW2:" << this->limitSwitchGPIOs->switchPin2.isOn());

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
	this->setpointStepperValue = map(relativePosition,
									 (float)0.0,
									 (float)1.0,
									 (float)0.0,
									 (float)this->maxStepperValue);

	int diff = this->setpointStepperValue - this->currentStepCount;
	Stepper::OnUpdate(diff, enable);
}

bool StepperWithLimits::Calibrate(void)
{
    eStepperCmd command;

    this->calibrationState = transitionFunction(this->calibrationState,
                                                this->getPosition(),
                                                &command);

    //LOG("Calibration state:" <<  calibrationState);

    switch(calibrationState)
    {
        case eState_Left:
        	//assert(this->currentStepCount == 0); // TODO remove this assertion
            break;
        case eState_Right:
            this->maxStepperValue = this->currentStepCount;
            LOG("max stepper value" << this->maxStepperValue);

            break;
        default:
            break;
    }

    switch(command)
    {
    case eStepperCmd_GoToLeft:
    	Stepper::OnUpdate(-1, true);
    	Stepper::OnUpdateRegulation();
        break;

    case eStepperCmd_GoToRight:
    	Stepper::OnUpdate(1, true);
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

/////////// PlotterArm



PlotterArm::PlotterArm(const PlotterArmConfig _armConfig)
	:minAngle(_armConfig.angleMin),
	maxAngle(_armConfig.angleMax),
	armAngleOffset(_armConfig.angleOffset)
{}


void PlotterArm::NewPosition(PlotterArmSetting &setting)
{
	float requiredAngle = toPositiveAngle(setting.setpointAngle - this->armAngleOffset);
	requiredAngle = constrain(requiredAngle, this->minAngle, this->maxAngle);

	float relativePosition = map(requiredAngle,
                                  (float)this->minAngle,
                                  (float)this->maxAngle,
                                  (float)0.0,
                                  (float)1.0);
	StepperWithLimits::OnUpdate(relativePosition, setting.enable);
}


