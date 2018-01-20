/*
 * communication.c
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#include "communication.h"

//TODO remove this
#include <iostream>
#include <stdint.h>
#include <cmath>
//#include "database.h"
//#include "myGlobal.h"

#include "app_threads.h"

#define TRACE std::cout << __FUNCTION__ << std::endl
void error(const char *msg)
{
    std::cout << msg<< std::endl;;
    //exit(1);
}

Communication::Communication()
{
    portno = 3000;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    while(bindToPort() == false);

//    close(newsockfd);
//    close(sockfd);
}

bool Communication::bindToPort()
{
    TRACE;
    //bzero((char *) &serv_addr, sizeof(serv_addr));
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
        portno++;
        return false;
    }

    return true;
}

bool Communication::listen()
{
    std::cout << "Listening on port: " << portno << std::endl;

    ::listen(sockfd,5);
    socklen_t clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
    {
        error("ERROR on accept");
        return false;

    }
    return true;
}

bool Communication::sendData(uint8_t *data, int dataLength)
{
    TRACE;
    return false;
};

bool Communication::recvData(uint8_t *data, int *pDataLength)
{
    TRACE;

    int maxDataSize = std::min(*pDataLength, (int)sizeof(buffer));

    bzero(buffer, sizeof(buffer));
    //LOG(maxDataSize);
    int n = read(newsockfd, buffer, maxDataSize);

    if (n <= 0)
    {
        error("ERROR reading from socket");
        return false;
    }

    memcpy(data, buffer, n);
    *pDataLength = n;

    return true;
}
