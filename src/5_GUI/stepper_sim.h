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

#include "draw.h"
#include "gui.h"

// TODO Poradne prepracovat tuhle tridu - je mi k necemu angleOffset?

class StepperGui
{
    position rotationCenter;
    position endPoint;
    int armLength;
    float angle;

    int armAngleOffset;

    int minAngle;
    int maxAngle;

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

       minAngle = 0;
       maxAngle = 180;

       stepCount = 0;
       angle = 0;

       endPoint.x = 0;
       endPoint.y = 0;
       endPoint.z = 0;

       stepsPerOneTurn = 1000;
       gearRatio = 10;

       directionLeft = true;

       this->update();
    };

    void draw(void)
    {
        for (int i = 0; i < 6; i++)
        {
            //drawCircle(getCirclePosition(rotationCenter, armLength,(float)( 180 - (i * 45))), 2);
        }

        //endPoint = getCirclePosition(rotationCenter, armLength, 180 - angle);
        drawLine(rotationCenter, endPoint);
        //drawCircle(rotationCenter, 10);
    }

    position getEndPoint(void)
    {
        return this->endPoint;
    }

    void setCount(int stepCount)
    {
    	this->stepCount = stepCount;
    }

    void setAngle(float newAngle)
    {
    	this->angle = newAngle;
    	//this->stepCount = (newAngle *  (gearRatio * stepsPerOneTurn)) / (float)360;
    }


    void _increase(void)
    {
        this->stepCount += 1;
        this->update();
    }

    void _decrease(void)
    {
        this->stepCount -= 1;
        this->update();
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
            this->stepCount -= 1;
        }
        else
        {
            //Increase position
            this->stepCount += 1;
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
        if ((angle < minAngle) || (angle > maxAngle))
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

        //this->angle = ((float)360 * stepCount / (gearRatio * stepsPerOneTurn));

        //int cycleOffset = (int)this->angle / 360;
        //this->angle -= cycleOffset * 360;

        //float tmp = (float)180 - angle + (float)armAngleOffset;
        endPoint = getCirclePosition(rotationCenter, armLength, this->angle);
    }
};

#endif /* STEPPERSIM_H_ */
