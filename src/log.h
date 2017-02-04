#pragma once

#include <iostream>
#include <sstream>
#include <thread>

#define LOG(message) {						\
	std::ostringstream os;					\
	os << std::this_thread::get_id() << " ";		\
	os << message << std::endl;				\
	std::cout << os.str();					\
 }
