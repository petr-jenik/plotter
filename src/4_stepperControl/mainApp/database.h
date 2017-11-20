/*
 * database.h
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include "IDatabase.h"
#include "stepperConfig.h"
#include "config.h"

#pragma pack(push, 1) // exact fit - no padding
typedef struct
{
    StepperSetting data[eStepperMaxCount];
} sDatabasePacket;
#pragma pack(pop)

class Database : public IDatabase<StepperSetting>
{
private:
    StepperSetting stepperData[eStepperMaxCount];
    bool used[eStepperMaxCount];

public:
    Database();
    virtual int allocate(void);
    bool setData(int Id, StepperSetting & data);
    StepperSetting * getData(int Id);
    bool processInputData(void * data, int dataLength);
};


#endif /* DATABASE_H_ */
