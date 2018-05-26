/*
 * Serial.h
 *
 *  Created on: 23. 5. 2018
 *      Author: apollo
 */

#ifndef xyz
#define xyz


class Serial
{
public:
	Serial();
	bool available();
	char read();
	void write(char sendChar);
	//void writeX(char *string);
};

#endif /* OS_EMBEDDED_SERIAL_H_ */
