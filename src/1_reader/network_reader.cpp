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

#include "app_threads.h"
#include "communication.h"
#include "queue.h"

Communication comm;
Queue queue;

void reader_init(void)
{
}

#define MAX_LINE_LENGTH 512
/*
void reader_readAndProcessFile(std::string fileName)
{
	DBG("Start server and listen for data" << std::endl);

	//std::cout << "Input file opened" << std::endl;

	while(comm.listen())
	{
		char tmpBuffer[MAX_LINE_LENGTH] = {0};
		int lineNumber = 0;


		int maxDataSize = std::min(getFreeSpaceSize(), sizeof(tmpBuffer));

		// New client connected
		while(comm.recvData((uint8_t*)tmpBuffer, &maxDataSize))
		{
			for (int i = 0; i < maxDataSize; i++)
			{
				push(tmpBuffer[i]);
			}
			memset(tmpBuffer, 0, sizeof(tmpBuffer));

			char oneLine[MAX_LINE_LENGTH] = {0};
			char byte;
			uint32_t idx = 0;

			while((idx < MAX_LINE_LENGTH) &&
					pop(oneLine[idx++]) &&
					oneLine[idx] != '\n'){};

			std::string line;
			line.assign(oneLine, idx);
			parser_update(line);

			LOG("Line number: " << lineNumber++ << ", data: " << line);

			maxDataSize = std::min(getFreeSpaceSize(), sizeof(tmpBuffer));
		}
	}
	std::cout << "END OF COMMUNICATION" << std::endl;
}
*/

void _delay(volatile uint32_t cycles)
{
	while(cycles--){};
}

uint32_t lineNumber = 0;

void processLine(char * buffer, uint32_t bufferSize)
{
	std::string line;
	line.assign(buffer, bufferSize);
	parser_update(line);

	LOG("Line number: " << lineNumber++ << ", data: " << line);
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
		if (comm.recvData((uint8_t*)buffer, &maxDataSize) == false)
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

		LOG("numberOfPushes: " << queue.numberOfPushes << std::endl);
		LOG("numberOfPops: " << queue.numberOfPops << std::endl);

		_delay(1<< 16);

		//parser_update(line);

	}
}

void reader_readAndProcessFile(std::string fileName)
{
	DBG("Start server and listen for data" << std::endl);

	while(comm.listen())
	{
		serveClient();
	}
}
