/*
 * steper.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#include "math_tools.h"

//#include "stepperConfig.h"
#include "config.h"
#include "calibration.h"
#include "Timer.h"

#include "hwStepperPins.h"
#include "plotterArm.h"
#include "global.h"

PlotterArm::PlotterArm(const ArmConfig _armConfig)
	:gpio(NULL),
	 maxAngle(_armConfig.angleMax),
	 minAngle(_armConfig.angleMin),
	 armAngleOffset(_armConfig.angleOffset)
{
    this->actualStepperValue = 0;
    this->maxStepperValue = 0;
    this->setpointStepperValue = 0;

    this->calibrationEnabled = false;
    this->calibrationState = eState_Start;

    this->directionLeft = false;
    this->makeStep = false;
    this->enableFlag = false;

    }

void PlotterArm::registerGPIOs(StepperGPIOs * gpio)
{
    this->calibrationEnabled = true;

	this->gpio = gpio;

    // Initialize all GPIOs
    this->gpio->enablePin.powerUp();
    this->gpio->stepPin.powerUp();
    this->gpio->directionPin.powerUp();
    this->gpio->resetPin.powerUp();
    this->gpio->sleepPin.powerUp();


	this->gpio->switchPin1.powerUp();
	this->gpio->switchPin2.powerUp();

    //TODO Disable stepper
	//Enable stepper
    this->_sleep(false);
    this->_reset();
    this->_enable();
}

bool PlotterArm::Calibrate(void)
{

    if (this->calibrationEnabled == false)
    {
        return true;
    }

    eStepperCmd command;

    this->calibrationState = transitionFunction(this->calibrationState,
                                                this->_getPosition(),
                                                &command);

    LOG("Calibration state:" <<  calibrationState);

    switch(calibrationState)
    {
        case eState_Left:
            this->actualStepperValue = 0;
            break;
        case eState_Right:
            this->maxStepperValue = this->actualStepperValue;
            break;
        default:
            break;
    }

    switch(command)
    {
    case eStepperCmd_GoToLeft:
        this->makeStep = true;
        this->directionLeft = true;
        break;

    case eStepperCmd_GoToRight:
        this->makeStep = true;
        this->directionLeft = false;
        break;

    case eStepperCmd_Stop:
        this->makeStep = false;
        this->directionLeft = true;
        break;

    default:
        this->makeStep = false;
        break;
    }

    return (eState_Done == this->calibrationState);
}


void PlotterArm::OnUpdate(StepperSetting * pSetting)
{
    if (pSetting != NULL)
    {
    	float requiredAngle1 = toPositiveAngle(pSetting->setpointAngle - this->armAngleOffset);
    	float requiredAngle = constrain(requiredAngle1, this->minAngle, this->maxAngle);
    	if (requiredAngle1 != requiredAngle)
    	{
    		int tmp = 42;
    	}
    	//if (requiredAngle - (pSetting->setpointAngle - this->armAngleOffset))
    	//{
    		//LOG(requiredAngle - (pSetting->setpointAngle - this->armAngleOffset));
			//LOG("required angle:" << requiredAngle << ", setting:" << pSetting->setpointAngle - this->armAngleOffset << ", range: " << this->minAngle << ", " << this->maxAngle);
    		//while(1)
    		//{
    		//}
    	//}
		float tmp = map(requiredAngle, this->minAngle, this->maxAngle, (float)0, (float)this->maxStepperValue);
		this->setpointStepperValue = (int)(tmp);

        /*
        std::cout << __FUNCTION__ << std::endl;
        std::cout << "pSetting->setpointAngle:  " <<  pSetting->setpointAngle << "°" << std::endl;
        std::cout << "requiredAngle:  " <<  requiredAngle << "°" << std::endl;
        std::cout << "max step count: " <<  this->maxStepperValue << "steps" <<std::endl;
        std::cout << "setpointValue: " <<  this->setpointStepperValue << "steps" << std::endl;
        */
        assert(this->setpointStepperValue >= 0);
        assert(this->setpointStepperValue <= this->maxStepperValue);

        // Enable or disable stepper controller - enable only once
        if (this->enableFlag != pSetting->enable)
        {
            if (true == pSetting->enable)
            {
                this->_enable();
            }
            else
            {
                this->_disable();
            }
            this->enableFlag = pSetting->enable;
        }
    }
}


void PlotterArm::OnUpdateRegulation(void)
{
    if (false == this->enableFlag)
    {
        this->makeStep = false;
        return;
    }

    int error = this->getError();
    if (error == 0)
    {
        this->makeStep = false;
        return;
    }

    if (error > 0)
    {
        this->directionLeft = false;
    }
    else
    {
        this->directionLeft = true;
    }

    this->makeStep = true;
}

void PlotterArm::moveStart(void)
{
    if (true == this->makeStep)
    {
        if (true == this->directionLeft)
        {
            this->actualStepperValue -= this->stepSize;
            this->gpio->directionPin.turnOn();
        }
        else
        {
            this->actualStepperValue += this->stepSize;
            this->gpio->directionPin.turnOff();
        }
        this->gpio->stepPin.turnOn();
    }
}

void PlotterArm::moveEnd(void)
{
    if (true == this->makeStep)
    {
        this->gpio->stepPin.turnOff();
    }
}

int PlotterArm::getError(void)
{
    int error = this->setpointStepperValue - this->actualStepperValue;
    return error;
}

void PlotterArm::_enable(void)
{
    this->gpio->enablePin.turnOn();
}

void PlotterArm::_disable(void)
{
    this->gpio->enablePin.turnOff();
}

void PlotterArm::_reset(void)
{
    this->gpio->resetPin.turnOn();

    //TODO Move all delay times to named constants
    Timer::sleep(5);

    this->gpio->resetPin.turnOff();
}

void PlotterArm::_sleep(bool state)
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

eStepperPosition PlotterArm::_getPosition(void)
{
    eStepperPosition position = eStepperPosition_Undef;

    LOG("SW1: " << switchPin1.isOn() << ", SW2:" << switchPin2.isOn());

    bool status = gpio->switchPin1.isOn();
    status |= gpio->switchPin2.isOn();

    if (status == true)
    {
        position = eStepperPosition_AtLimitSwitch;
    }
    return position;
}
