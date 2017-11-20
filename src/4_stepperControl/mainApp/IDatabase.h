/*
 * IDatabase.h
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#ifndef IDATABASE_H_
#define IDATABASE_H_

const int maxDataCount = 10;

template <class Type> class IDatabase
{
public:
    virtual ~IDatabase(){};

    virtual int allocate(void) = 0;
    virtual bool setData(int Id, Type & data) = 0;
    virtual Type * getData(int Id) = 0;
    virtual bool processInputData(void * data, int dataLength) = 0;
};

#endif /* IDATABASE_H_ */
