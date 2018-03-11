/*
 * communication.h
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Communication
{
private:
    int portno ;
    char buffer[1024];
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    int sockfd;
    int newsockfd;

    bool bindToPort();

public:
    Communication();
    //~Communication();
    bool sendData(uint8_t *data, int dataLength);
    bool recvData(uint8_t *data, int * dataLength);
    bool listen();
};

#endif /* COMMUNICATION_H_ */
