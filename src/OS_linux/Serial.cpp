/*
 * Serial.cpp
 *
 *  Created on: 23. 5. 2018
 *      Author: apollo
 */

#include "Serial.h"
#include <iostream>


Serial::Serial()
{
}

bool Serial::available()
{
	return true;
}

char Serial::read()
{
	char rx;
	std::cin >> rx;
	return rx;
}

void Serial::write(char sendChar)
{
	std::cout << sendChar;
}
/*
void Serial::writeX(char *string)
{
	std::cout << string;
}
*/
