/*
 * network_reader.cpp
 *
 *  Created on: 20. 1. 2018
 *      Author: apollo
 */

#include <iostream>
#include <fstream>
#include <cstdio>
#include <errno.h>
#include <stdint.h>

#include "app_threads.h"
#include "communication.h"

Communication comm;

void reader_init(void)
{
}

#define MAX_LINE_LENGTH 512

void reader_loop(safe_queue<std::string> &queueOutput)
{
	DBG("Start server and listen for data" << std::endl);

	//std::cout << "Input file opened" << std::endl;
	char buffer[MAX_LINE_LENGTH] = {0};
	int dataLen = sizeof(buffer);
	int lineNumber = 0;
	while(comm.listen())
	{
		// New client connected
		while(comm.recvData((uint8_t*)buffer, &dataLen))
		{
			std::string line;
			line.assign(buffer,dataLen);//((const)&buffer[0]);
			queueOutput.send(line);
			//LOG("Reading finished, dataLen:");
			//LOG(dataLen);
			//LOG(", data: ");
			//LOG(line);
			LOG("Line number: " << lineNumber++ << ", data: " << line);

			dataLen = sizeof(buffer);
			memset(buffer, 0, sizeof(buffer));
		}
	}
	std::cout << "END OF COMMUNICATION" << std::endl;
}
