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

char Serial::readX()
{
	return uartGetChar();
}

void Serial::writeX(char sendChar)
{
	uartSendChar(sendChar);
	//return sizeof(sendChar);
}

void Serial::writeX(char *string)
{
	uartPrint(string);
}

