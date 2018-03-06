#pragma once

//#define LOG(message)
//#define DBG(message)


#include <iostream>
#include <sstream>
#include <thread>
/*
#define LOG(message) {						\
	std::ostringstream os;					\
	os << message << std::endl;				\
	std::cout << os.str();					\
 }
*/

#define LOG(message) std::cout << message << std::endl
/*
#define LOG(message) {						\
	std::ostringstream os;					\
	os << message << std::endl;				\
	std::cout << os.str();					\
 }
*/
#define DBG(message) {                      \
	std::ostringstream os;                  \
	os << message << std::endl;	            \
	std::cout << os.str();                  \
	}
