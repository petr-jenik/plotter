#pragma once

#include "embedded_cout.h"
#include <string.h>
#define LOG(message) (dbgCout << message << dbgEndl)

#define DBG(message) //(dbgCout << "DBG - " << message << dbgEndl)


class Trace
{
public:
	char buffer[100];

	explicit Trace(const char * name)
	{
		strncpy(buffer, name, sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';
		LOG("trace start:" << name);
	}

	~Trace()
	{
		LOG("trace end:" << buffer);
	}
};

//#define TRACE Trace _trace(__FUNCTION__);
#define TRACE
