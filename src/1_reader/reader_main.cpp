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

#include <algorithm>

#include "reader_main.h"
#include "parser_main.h"
#include "communication.h"
#include "queue.h"

#include "global.h"

Communication comm;
Queue queue;

void reader_init(void)
{
}

#define MAX_LINE_LENGTH 512

uint32_t lineNumber = 0;

void processLine(char * buffer, uint32_t bufferSize)
{
	parser_update(buffer, bufferSize);

	LOG("Line number: " << lineNumber++ << ", data: " << buffer);
}


bool readLineFromQueue(Queue &queue, char *pBuffer , uint32_t & sizeOfBuffer)
{
	char line[MAX_LINE_LENGTH] = {0};
	bool endOfLineFound = false;
	int bufferSize = std::min((size_t)sizeOfBuffer, sizeof(line));
	int minDataSize = std::min(queue.getNumberOfItems(), (size_t)bufferSize);
	int idx = 0;

	bool retval = false;

	// Process the whole queue
	while((idx < minDataSize) &&
		  (endOfLineFound == false) &&
		  (queue.getNumberOfItems() != 0))
	{
		queue.pop(line[idx]);

		if ('\n' == line[idx])
		{
			endOfLineFound = true;
		}

		idx++;
	}

	if (endOfLineFound)
	{
		sizeOfBuffer = idx;
		memcpy(pBuffer, line, (size_t)idx);
		retval = true;
	}
	else
	{
		LOG("End of line was not found");

		size_t numberOfItems = queue.getNumberOfItems();

		for (int i = 0; i < idx; i++)
		{
			queue.push(line[i]);
		}

		for (int i = 0; i < numberOfItems; i++)
		{
			char byte;
			queue.pop(byte);
			queue.push(byte);
		}

		retval = false;
		sizeOfBuffer = 0;
	}

	return retval;
}

void serveClient()
{
	int lineNumber = 0;

	char buffer[MAX_LINE_LENGTH] = {0};

	// Loop for data reading from client
	bool receiveIsActive = true;
	while(receiveIsActive)
	{
		memset(buffer, 0, sizeof(buffer));
		int maxDataSize = std::min(queue.getFreeSpaceSize(), sizeof(buffer));

		// New client connected
		if (gcodeRecvData((uint8_t*)buffer, &maxDataSize) == false)
		{
			//receiveIsActive = false;
			// Error during reading, break internal loop
			receiveIsActive = false;
		}

		LOG("received data size: " << maxDataSize);

		// Push received data to data FIFO (queue)
		for (int i = 0; i < maxDataSize; i++)
		{
			queue.push(buffer[i]);
		}

		memset(buffer, 0, sizeof(buffer));
		uint32_t dataSize = sizeof(buffer);
		while(readLineFromQueue(queue, buffer, dataSize))
		{
			processLine(buffer, maxDataSize);
			memset(buffer, 0, sizeof(buffer));
			dataSize = sizeof(buffer);
		}

		LOG("numberOfPushes: " << queue.numberOfPushes);
		LOG("numberOfPops: " << queue.numberOfPops);

		//parser_update(line);

	}
}

void reader_readAndProcess()
{
	DBG("Start server and listen for data");

	while(comm.listen())
	{
		serveClient();
	}
}
