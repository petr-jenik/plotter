/*
 * fake_reader.cpp
 *
 *  Created on: 24. 2. 2018
 *      Author: apollo
 */

#include "log.h"
#include "reader_main.h"
//#include "gcode.h"
#include <stdint.h>
#include "Timer.h"

//#include <algorithm>

//#include "reader_main.h"
#include "parser_main.h"
//#include "queue.h"

#include "global.h"

#include "hwUart.h"

const uint32_t gcommandMaxLen = 100;

const eUart gcUartChannel = cUART6;

typedef struct
{
	char data[gcommandMaxLen + 1]; // One extra character for string termination
	int dataLen;
	bool used;
} GcodeCommand;

template <class T> class Queue
{
private:
	static const uint32_t cQueueLen = 50;
	T items[cQueueLen];
	uint32_t dataStart;
	uint32_t numberOfItems;

public:
	uint32_t numberOfPushes;
	uint32_t numberOfPops;

	Queue():
		dataStart(0),
		numberOfItems(0),
		numberOfPushes(0),
		numberOfPops(0)
	{
		for (uint32_t i = 0; i < cQueueLen; i++)
		{
			memset(&this->items[i], 0, sizeof(T));
		}
	}

	bool push(T item)
	{
		bool retval = false;
		if (numberOfItems < cQueueLen)
		{
			this->items[(dataStart + numberOfItems) % cQueueLen] = item;
			numberOfItems++;
			retval = true;
			numberOfPushes++;
		}
		return retval;
	}

	bool pop(T& item)
	{
		bool retval = false;
		if (numberOfItems > 0)
		{
			item = items[dataStart];
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

Queue<GcodeCommand> queue;

bool addCommandToQueue(const uint8_t* data, uint32_t dataLen)
{
	GcodeCommand gcmd;

	bool result = false;

	if (dataLen <= gcommandMaxLen)
	{
		// Set last character to '\0' just to be sure
		gcmd.data[sizeof(gcmd.data) - 1] = '\0';

		memcpy(gcmd.data, data, dataLen);
		gcmd.dataLen = dataLen;
		//_DISABLE_INTERRUPT();
		result = queue.push(gcmd);

		if (result == true)
		{
			// ACK means that command was added to the queue
			uartPrint("ACK\n", gcUartChannel);
		}
		else
		{
		    // Queue is full, try it in a while
			uartPrint("ERR\n", gcUartChannel);
		}


        //size_t freeItems = queue.getFreeSpaceSize();
		//if (freeItems > 0)
		//{
		//	// ACK means that there is a place at least for one more command
		//	uartPrint("ACK\n", gcUartChannel);
		//}
		//else
		//{

		//}

		//_ENABLE_INTERRUPT();
	}
	else
	{
		result = false;
	}
	return result;
}

uint8_t data[gcommandMaxLen] = {0};
uint32_t dataIndex = 0;
uint32_t errorCountQueueFull = 0;
uint32_t errorCountOverrun = 0;

void rxCallback(uint8_t byte, RxState state)
{
	if (state == State_ERROR)
	{
		errorCountOverrun++;
		dataIndex = 0;
		return;
	}

	data[dataIndex] = byte;
	dataIndex++;
	//LOG("Data: " << (const char *)data);

	// TODO - Use only \n
	if ((byte == '\n')/* or (byte == '\r')*/ or (dataIndex >= gcommandMaxLen))
	{
		bool result = addCommandToQueue(data, dataIndex);
		if (result == false)
		{
			errorCountQueueFull++;
		}
		dataIndex = 0;
	}
}

void reader_init(void)
{
	//HW specific
	UartConfig config;
	config.baudrate = 115200;

	uartInit(gcUartChannel, config);

	uartRegisterRxCallback(rxCallback, gcUartChannel);
}


uint32_t lineNumber = 0;

void processLine(char * buffer, uint32_t bufferSize)
{
	parser_update(buffer, bufferSize);

	LOG("Line number: " << lineNumber++ << ", data: " << buffer);
}


/*
void test()
{
	std::string line;
	std::getline (std::cin, line);

	addCommandToQueue((const uint8_t*)line.c_str(), line.length());
}*/

void reader_readAndProcess()
{
	while(1)
	{
		GcodeCommand gcmd;

		//HAL_NVIC_DisableIRQ(USART6_IRQn);
		bool cmdReceived = queue.pop(gcmd);
		//_ENABLE_INTERRUPT();

		if (cmdReceived)
		{
			processLine(gcmd.data, gcmd.dataLen);
			LOG("numberOfPushes: " << queue.numberOfPushes);
			LOG("numberOfPops: " << queue.numberOfPops);
		}
		else
		{
			static uint32_t cycleCount = 0;
			cycleCount++;
			if (cycleCount > 100000)
			{
				cycleCount = 0;
				//LOG("numberOfPops: " << queue.numberOfPops);
				LOG("numberOfErrors queue full: " << errorCountQueueFull);
				LOG("numberOfErrors overrun: " << errorCountOverrun);
			}
		}
		//Timer::sleep(10);
	}
}
