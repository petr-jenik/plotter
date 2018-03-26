/*
 * stepperSim.h
 *
 *  Created on: Jan 20, 2016
 *      Author: apollo
 */

#ifndef STEPPERSIM_H_
#define STEPPERSIM_H_


/*
 * stepperSim.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: apollo
 */

#include <algorithm>

#include "draw.h"
#include "gui.h"
#include "stepperConfig.h"

class StepperGui
{
    position rotationCenter;
    position endPoint;
    int armLength;
    float angle;

    int armAngleOffset;

    float minAngle;
    float maxAngle;

    int stepCount; //Number of steps

    bool directionLeft;

    int stepsPerOneTurn;
    int gearRatio; //(number of stepper turns required for one turn of plotter arm)

public:
    StepperGui(int x, int y, int _armLength, int _armAngleOffset)
    {
       armLength = _armLength;
       armAngleOffset = _armAngleOffset;
       rotationCenter.x = x;
       rotationCenter.y = y;

       minAngle = MIN_ANGLE;

       //TODO Why "-1" ????
       maxAngle = MAX_ANGLE;

       stepCount = 0;
       angle = 0;

       endPoint.x = 0;
       endPoint.y = 0;
       endPoint.z = 0;

       stepsPerOneTurn = 4000;
       gearRatio = 10;
       directionLeft = true;

       this->update();
    };

    void draw(void)
    {
        drawCircle(getCirclePosition(rotationCenter, armLength, 0), 2);
        drawCircle(getCirclePosition(rotationCenter, armLength, 90), 2);
        drawCircle(getCirclePosition(rotationCenter, armLength, 180) , 2);

        drawCircle(getCirclePosition(rotationCenter, armLength, 45), 2);
        drawCircle(getCirclePosition(rotationCenter, armLength, 135) , 2);


        position needle = getCirclePosition(rotationCenter, 30, this->angle);
        drawLine(rotationCenter, needle);

        /*
    	for (int i = 0; i < 5; i++)
        {
            drawCircle(getCirclePosition(rotationCenter, armLength,(float)( 180 - (i * 45))), 2);
        }*/

        //endPoint = getCirclePosition(rotationCenter, armLength, 180 - angle);
        drawLine(rotationCenter, endPoint);
        drawCircle(rotationCenter, 10);
    }

    position getEndPoint(void)
    {
        return this->endPoint;
    }

    void setDirection(bool directionLeft)
    {
        this->directionLeft = directionLeft;
    }

    bool getDirection(void)
    {
        return this->directionLeft;
    }

    void move(void)
    {
        if (getDirection() == true)
        {
            //Decrease position
            this->stepCount -= STEP_SIZE;
        }
        else
        {
            //Increase position
            this->stepCount += STEP_SIZE;
        }
        this->update();

    }

    float getAngle(void)
    {
        return this->angle;
    }

    bool isAtLimitPosition(void)
    {
        int angle = getAngle();
        if ((angle <= minAngle) || (angle >= maxAngle))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void update()
    {
        //if ( 0 <= rotation && 100 >= rotation)
        //{
            //this->stepCount = rotation;
        //}

        float tmpAngle = ((float)(360 * stepCount) / (gearRatio * stepsPerOneTurn));

        //std::cout << "Angle: " << tmpAngle << std::endl;
        //std::cout << "Step count: " << stepCount << std::endl;

        //tmpAngle = std::max(tmpAngle, this->minAngle);
        //tmpAngle = std::min(tmpAngle, this->maxAngle);

        //this->angle = tmpAngle + (float)armAngleOffset;
        this->angle = tmpAngle;
        //float tmp = (float)180 - angle + (float)armAngleOffset;
        //stepCount = (tmpAngle * (gearRatio * stepsPerOneTurn))/(float)360;

        //int cycleOffset = (int)this->angle / 360;
        //this->angle -= cycleOffset * 360;

        //float tmp = (float)180 - angle + (float)armAngleOffset;
        endPoint = getCirclePosition(rotationCenter, armLength, this->angle + (float)armAngleOffset);
    }
};

#endif /* STEPPERSIM_H_ */
