#include "hwUart.h"

#include "HardwareSerial.h"

void uartInit()
{
	Serial.begin(8 * 115200);
   	return;
}

char uartGetChar(void)
{
	return Serial.read();
}

void uartSendString(const char * pString)
{
	Serial.write(pString);
}

void uartSendChar(char sendChar)
{
	Serial.write(sendChar);
}

