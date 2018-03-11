#pragma once

#ifdef OS_EMBEDDED

#define LOG(message)
#define DBG(message)

#endif //OS_EMBEDDED
#ifdef OS_LINUX

//#include <iostream>
//#include <sstream>
//#include <thread>
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

std::ostream& operator << (std::ostream& stream, position pos);
std::ostream& operator << (std::ostream& stream, const moveCommand& cmd);
std::ostream& operator << (std::ostream& stream, const armCommand& cmd);

#endif //OS_LINUX
