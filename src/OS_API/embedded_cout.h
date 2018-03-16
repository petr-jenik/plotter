/*
 * embedded_cout.h
 *
 *  Created on: 15 Mar 2018
 *      Author: pi
 */

#ifndef OS_API_EMBEDDED_COUT_H_
#define OS_API_EMBEDDED_COUT_H_

typedef enum
{
	dbgEndl = 0
} eStreamControl;


class EmbeddedCout
{
private:
	const static int cEmbeddedCoutBufferSize = 512;
	char buffer[cEmbeddedCoutBufferSize];
	int charIndex = 0;

	void _putChar(char c);
	size_t _getFreeSpaceSize();
	void _add(const char * string, const size_t stringLen);

public:
	EmbeddedCout();
	void flush();

	EmbeddedCout& operator<<(char * c_string);
	//EmbeddedCout& operator<<(std::string data);
	EmbeddedCout& operator<<(eStreamControl command);
	EmbeddedCout& operator<<(const int data);
	EmbeddedCout& operator<<(const unsigned int data);
	EmbeddedCout& operator<<(const float fData);
};

extern EmbeddedCout dbgCout;

#endif /* OS_API_EMBEDDED_COUT_H_ */
