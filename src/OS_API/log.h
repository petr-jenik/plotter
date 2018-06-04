#pragma once

#include "embedded_cout.h"

#define LOG(message) (dbgCout << message << dbgEndl)

#define DBG(message) //(dbgCout << "DBG - " << message << dbgEndl)


class Trace
{
public:
	//char buffer[100];
	const char * pName;
	Trace(const char * name)
	{
		pName = name;
		LOG("trace start:" << pName);
	}

	~Trace()
	{
		LOG("trace end:" << pName);
	}
};

//#define TRACE Trace _trace(__FUNCTION__);
#define TRACE
