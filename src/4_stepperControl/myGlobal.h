/*
 * myGlobal.h
 *
 *  Created on: Dec 26, 2015
 *      Author: apollo
 */

#ifndef MYGLOBAL_H_
#define MYGLOBAL_H_

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stdint.h>

#define assert(x) if(!(x))	                                            \
	              {                                                     \
						while(1)                                        \
						{                                               \
							std::cout<< "ASSERT FAILED!" << std::endl;  \
						}                                               \
                   }

#define MIN(x,y) ((x) < (y)) ? (x) : (y)
#define MAX(x,y) ((x) > (y)) ? (x) : (y)

#define ARRAY_SIZE(x) (sizeof((x))/sizeof((x)[0]))

#define TO_UPPER(x) (((x) >= 'a') && ((x) <= 'z')) ? ((x) + 'A' - 'a') : (x)

//#include <cstdint>
//#include <stdint.h>
//#include <cstring>
//#include <iostream>
//#include <cstdlib>

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* MYGLOBAL_H_ */
