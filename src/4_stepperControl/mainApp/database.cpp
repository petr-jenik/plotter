/*
 * database.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#include "database.h"
#include "myGlobal.h"
#include <string.h>

Database::Database()
    {
        for (int i = 0; i < eStepperMaxCount; i++)
        {
            used[i] = false;
            memset(&stepperData[i], 0, sizeof(StepperSetting));
        }
    };

// Allocate
int Database::allocate(void)
{
    int i = 0;
    for (i = 0; i < eStepperMaxCount; i++)
    {
        if (false == used[i])
        {
            used[i] = true;
            return i;
        }
    }
    return -1;
}

bool Database::setData(int Id, StepperSetting & data)
{
    if ((0 <= Id) && (eStepperMaxCount > Id))
    {
        this->stepperData[Id] = data;
        return true;
    }
    else
    {
        return false;
    }
}

StepperSetting * Database::getData(int Id)
{
    if ((0 <= Id) && (eStepperMaxCount > Id))
    {
        return &this->stepperData[Id];
    }
    else
    {
        return NULL;
    }
}

bool Database::processInputData(void * data, int dataLength)
{
    if (dataLength != sizeof(sDatabasePacket))
    {
        return false;
    }

    sDatabasePacket * newData = (sDatabasePacket*)data;

    for(int i = 0; i < eStepperMaxCount; i++)
    {
        memcpy(&this->stepperData[i], &newData->data[i], sizeof(StepperSetting));
    }
    return true;
}
