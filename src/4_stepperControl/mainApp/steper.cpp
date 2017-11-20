/*
 * steper.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#include <plotterarm.h>
#include "hwStepperPins.h"
#include "Timer.h"
#include "calibration.h"

#include "stepperConfig.h"
#include "config.h"

#include "IDatabase.h"

using namespace std;

PlotterArm::PlotterArm(sStepperPins pinsDescription, const ArmConfig _armConfig, sSwichPins switchPins)
//PlotterArm::PlotterArm(sStepperPins pinsDescription, sSwichPins switchPins, float _arm_offset)
:directionPin(pinsDescription.directionPinDesc),
stepPin(pinsDescription.stepPinDesc),
resetPin(pinsDescription.resetPinDesc),
sleepPin(pinsDescription.sleepPinDesc),
enablePin(pinsDescription.enablePinDesc),
switchPin1(switchPins.limitSwitch1),
switchPin2(switchPins.limitSwitch2),
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
    //this->pDatabase = _pDatabase;
    //if(this->database != NULL)
    //{
        //this->Id = pDatabase->allocate();
        //controller.reg(this, eObjectType_stepper);
    //}

    // Initialize all GPIOs
    this->enablePin.powerUp();
    this->stepPin.powerUp();
    this->directionPin.powerUp();
    this->resetPin.powerUp();
    this->sleepPin.powerUp();

    //TODO Disable stepper
    //Enable stepper
    this->_sleep(false);
    this->_reset();
    this->_enable();

    if (this->switchPin1.isValid())
    {
        this->calibrationEnabled = true;
        this->switchPin1.powerUp();
        this->switchPin2.powerUp();
    }
}

/*
Stepper::Stepper(IEventHandler &handler, sStepperPins gpios)
:directionPin(gpios.directionPinDesc),
stepPin(gpios.stepPinDesc),
resetPin(gpios.resetPinDesc),
sleepPin(gpios.sleepPinDesc),
enablePin(gpios.enablePinDesc),
switchPin1(undefPin),
switchPin2(undefPin)
{
    this->_init(handler);
}
*/

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
#ifdef SERIAL_DEBUG
    serialPrintf("Stepper %d, calibration state: %d\n", Id, calibrationState);
#endif //SERIAL_DEBUG

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

/*
void Stepper::OnUpdate(void)
{
    StepperSetting * pSetting = pDatabase->getData(this->Id);

    if (pSetting != NULL)
    {
        this->setpointValueInPercent = pSetting->value;

        this->setpointValue = this->setpointValueInPercent * this->maxValue / maxSetpointValueInPercent;

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
*/

void PlotterArm::OnUpdate(StepperSetting * pSetting)
{
    if (pSetting != NULL)
    {
        //pSetting->value; //setpointValueInPercent
    	float requiredAngle = constrain(pSetting->setpointAngle - this->armAngleOffset, this->minAngle, this->maxAngle);
        this->setpointStepperValue = (int)map(requiredAngle, this->minAngle, this->maxAngle, (float)0, (float)this->maxStepperValue);

        std::cout << __FUNCTION__ << std::endl;
        std::cout << "pSetting->setpointAngle:  " <<  pSetting->setpointAngle << "°" << std::endl;
        std::cout << "requiredAngle:  " <<  requiredAngle << "°" << std::endl;
        std::cout << "max step count: " <<  this->maxStepperValue << "steps" <<std::endl;
        std::cout << "setpointValue: " <<  this->setpointStepperValue << "steps" << std::endl;

        assert(this->setpointStepperValue >= 0);
        assert(this->setpointStepperValue <= maxSetpointValueInPercent);

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
            this->directionPin.turnOn();
        }
        else
        {
            this->actualStepperValue += this->stepSize;
            this->directionPin.turnOff();
        }
        this->stepPin.turnOn();
    }
}

void PlotterArm::moveEnd(void)
{
    if (true == this->makeStep)
    {
        this->stepPin.turnOff();
    }
}

int PlotterArm::getError(void)
{
    int error = this->setpointStepperValue - this->actualStepperValue;
    return error;
}

void PlotterArm::_enable(void)
{
    this->enablePin.turnOn();
}

void PlotterArm::_disable(void)
{
    this->enablePin.turnOff();
}

void PlotterArm::_reset(void)
{
    this->resetPin.turnOn();

    //TODO Move all delay times to named constants
    Timer::sleep(5);

    this->resetPin.turnOff();
}

void PlotterArm::_sleep(bool state)
{
    if (true == state)
    {
        //Enable sleep mode
        this->sleepPin.turnOn();
    }
    else
    {
        //Disable sleep mode
        this->sleepPin.turnOff();
    }
}

eStepperPosition PlotterArm::_getPosition(void)
{
    eStepperPosition position = eStepperPosition_Undef;
#ifdef SERIAL_DEBUG
    serialPrintf("Stepper %d, SW1: %d\tSW2: %d\n", switchPin1.isOn(), switchPin2.isOn());
#endif //SERIAL_DEBUG

    bool status = switchPin1.isOn();
    status |= switchPin2.isOn();

    if (status == true)
    {
        position = eStepperPosition_AtLimitSwitch;
    }
    return position;
}
