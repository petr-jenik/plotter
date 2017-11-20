/*
 * mainApp.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#include <plotterarmController.h>
#include "myGlobal.h"

#include "database.h"
#include "hwStepperPins.h"
#include "servo.h"
#include "Timer.h"

#include "global.h"

#include "app_threads.h"

using namespace std;

//Communication communication;

const ArmConfig armConfigLeft = {0, 180, left_stepper_offset};
const ArmConfig armConfigRight = {0, 180, right_stepper_offset};

PlotterArm stepperLeft(getStepperPins(0), armConfigLeft, getSwitchPins(0));
PlotterArm stepperRight(getStepperPins(1), armConfigRight, getSwitchPins(1));
Servo servo(1);

ArmController stepperController(stepperLeft, stepperRight, servo);

bool isSystemReady()
{
	return stepperController.calibrationFinished();
}


void stepperControl_init(void)
{
    stepperController.calibrate(true);

    while(!stepperController.calibrationFinished())
    {
    		stepperController.calibrate(false);
    }

    // Go to the middle position
    //armCommand command;

    //command.relativeAngle1 = maxSetpointValueInPercent / 2;
    //command.relativeAngle2 = maxSetpointValueInPercent / 2;
    //command.relPosZ = maxSetpointValueInPercent / 2;
    //command.extrude = false;

    //database.setData(0, stepperSetting);
    //database.setData(1, stepperSetting);
    //stepperController.OnUpdateAll(command);
    //stepperController.OnMove();
}

//void stepperUpdate(armCommand newCommand)
void stepper_parseCommand(armCommand newCommand)
{
    stepperController.OnUpdateAll(newCommand);
    stepperController.OnMove();

/*
    if (counter == 3)
    {
        sStepperPins pins1 = getStepperPins(1);
        sStepperPins pins2 = getStepperPins(2);
        sStepperPins pins3 = getStepperPins(3);

        Gpio gpios[] =
        {
                {pins1.directionPinDesc},
                {pins1.enablePinDesc},
                {pins1.resetPinDesc},
                {pins1.sleepPinDesc},
                {pins1.stepPinDesc},
                {pins2.directionPinDesc},
                {pins2.enablePinDesc},
                {pins2.resetPinDesc},
                {pins2.sleepPinDesc},
                {pins2.stepPinDesc},
                {pins3.directionPinDesc},
                {pins3.enablePinDesc},
                {pins3.resetPinDesc},
                {pins3.sleepPinDesc},
                {pins3.stepPinDesc}
        };

        for(unsigned int i = 0; i < (sizeof(gpios)/sizeof(gpios[0])); i++)
        {
            gpios[i].powerUp();
        }

        while(1)
        {
            for(unsigned int i = 0; i < (sizeof(gpios)/sizeof(gpios[0])); i++)
            {
                gpios[i].toggle();
            }
            Timer::sleep(10);
        }
    }
    */
}
