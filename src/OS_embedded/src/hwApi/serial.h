/*
 * serial.h
 *
 *  Created on: 22. 9. 2015
 *      Author: z003jsvn
 */

#ifndef SERIAL_H_
#define SERIAL_H_


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void serialInit(void);
char serialGetChar(void);
void serialPutChar(char sendChar);
//void serialPutString(const char * data);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* SERIAL_H_ */
