/*
 * Serial.cpp
 *
 *  Created on: 23. 5. 2018
 *      Author: apollo
 */

#include "hwUart.h"
#include "Serial.h"

Serial::Serial()
{
	uartInit();
}

bool Serial::available()
{
	if (uartDataAvailable())
	{
		return true;
	}
	else
	{
		return false;
	}
}

char Serial::read()
{
	return uartGetChar();
}

void Serial::write(char sendChar)
{
	uartSendChar(sendChar);
	//return sizeof(sendChar);
}

void Serial::write(char *string)
{
	uartPrint(string);
}

