/*
 * gcode.h
 *
 *  Created on: 11. 4. 2018
 *      Author: apollo
 */

#ifndef SRC_1_READER_GCODE_H_
#define SRC_1_READER_GCODE_H_

#include <stdint.h>

bool gcodeRecvData(uint8_t *data, int * pDataLength);

void gcodeSetItemId(uint8_t itemId);

#endif /* SRC_1_READER_GCODE_H_ */
