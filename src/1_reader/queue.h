/*
 * queue.cpp
 *
 *  Created on: 7 Feb 2018
 *      Author: pi
 */

#pragma once

#include <stdint.h>
#include <cstring>

class Queue
{
private:
	static const uint32_t cQueueLen = 1001;
	char buffer[cQueueLen];
	uint32_t dataStart;
	uint32_t numberOfItems;

public:
	uint32_t numberOfPushes;
	uint32_t numberOfPops;

	Queue()
	{
		dataStart = 0;
		numberOfItems = 0;
		memset(buffer, 0, sizeof(buffer));

		numberOfPushes = 0;
		numberOfPops = 0;
	}

	bool push(char byte)
	{
		bool retval = false;
		if (numberOfItems < cQueueLen)
		{
			buffer[(dataStart + numberOfItems) % cQueueLen] = byte;
			numberOfItems++;
			retval = true;
			numberOfPushes++;
		}
		return retval;
	}

	bool pop(char& byte)
	{
		bool retval = false;
		if (numberOfItems > 0)
		{
			byte = buffer[dataStart];
			dataStart = (dataStart + 1) % cQueueLen;
			numberOfItems--;
			retval = true;
			numberOfPops++;
		}
		return retval;
	}

	size_t getFreeSpaceSize(void)
	{
		return cQueueLen - numberOfItems;
	}


	size_t getNumberOfItems(void)
	{
		return numberOfItems;
	}


};




