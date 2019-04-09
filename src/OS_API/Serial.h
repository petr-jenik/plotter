/*
 * Serial.h
 *
 *  Created on: 23. 5. 2018
 *      Author: apollo
 */

#ifndef OS_EMBEDDED_SERIAL_H_
#define OS_EMBEDDED_SERIAL_H_


class Serial
{
public:
	Serial();
	bool available();
	char read();
	void write(char sendChar);
	void write(char *string);
};

#endif /* OS_EMBEDDED_SERIAL_H_ */
