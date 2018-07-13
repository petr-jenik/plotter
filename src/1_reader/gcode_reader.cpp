/*
 * gcode_reader.cpp
 *
 *      Author: apollo
 */

#include "gcode.h"

#include "lopata_gcode.h"
#include "logo_gcode.h"

#include <stdint.h>

#ifdef OS_LINUX
	#include <string.h>
	#define memcpy_P memcpy
	#define PROGMEM
#elif OS_EMBEDDED
	#include <string.h>
	#define memcpy_P memcpy
	#define PROGMEM
#else
	#include <pgmspace.h>
#endif


typedef struct
{
    const unsigned char* pData;
    unsigned int len;
} Gcode;


extern const unsigned char lopata_gcode_data[] PROGMEM;
extern const unsigned int lopata_gcode_len;

extern const unsigned char logo_gcode_data[] PROGMEM;
extern const unsigned int logo_gcode_len;

Gcode getGcode(uint8_t itemId)
{
    Gcode retval;
    if (itemId == 1)
    {
        retval.pData = lopata_gcode_data;
        retval.len = lopata_gcode_len;
    }
    else
    {
        retval.pData = logo_gcode_data;
        retval.len = logo_gcode_len;
    }
    return retval;
}


unsigned int currentIdx = 0;

uint8_t gItemId = 0;

void gcodeSetItemId(uint8_t itemId)
{
        gItemId = itemId;
}


bool gcodeRecvData(uint8_t *data, int * pDataLength)
{
        Gcode gcode = getGcode(gItemId);
    
	unsigned int dataSize = gcode.len - currentIdx;
	unsigned int dataSizeToCopy = (*pDataLength < dataSize) ? *pDataLength : dataSize;
	memcpy_P(data, &gcode.pData[currentIdx], dataSizeToCopy);
	currentIdx += dataSizeToCopy;

	*pDataLength = dataSizeToCopy;

	if (currentIdx >= gcode.len)
	{
		currentIdx = 0;
	}
	return true;
}


 
