/*
 * communication.h
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "ICommunication.h"
#include "myStdlib.h"

class Communication : public ICommunication
{
private:
    char buffer[256];

public:
    Communication();
    //~Communication();
    bool sendData(uint8_t *data, int dataLength);
    bool recvData(uint8_t *data, int * dataLength);
};

#endif /* COMMUNICATION_H_ */
