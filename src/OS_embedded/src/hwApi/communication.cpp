/*
 * communication.c
 *
 *  Created on: April 5, 2016
 *      Author: apollo
 */

#include "communication.h"
#include "serial.h"
#include "database.h"
#include "myGlobal.h"

#include "myStdlib.h"
//#include "stdlib.h"
//#include <cstring>

#ifdef SERIAL_DEBUG
#define TRACE serialPrintf("\nTRACE: %s\n", __FUNCTION__)
#else
#define TRACE
#endif //SERIAL_DEBUG

void error(const char *msg)
{
#ifdef SERIAL_DEBUG
    serialPrintf("%s\n", msg);
#endif //SERIAL_DEBUG
}

int _getInputString(char * buffer, uint16_t bufferSize)
{
    uint16_t charIdx = 0;

    while(1)
    {
        if (charIdx > bufferSize - 1)
        {
            charIdx = 0;
            break;
        }

        char inputChar = myGetchar();

        if ((inputChar == '\r') || (inputChar == '\n'))
        {
            break;
        };

#ifdef SERIAL_DEBUG
        //TODO replace with serialPrintf
        serialPutChar(inputChar);
        //serialPrintf("%c", inputChar);
#endif //SERIAL_DEBUG

        buffer[charIdx] = TO_UPPER(inputChar);
        charIdx++;
    }
    buffer[charIdx] = '\0';

    return (int)charIdx;
}

Communication::Communication()
{
    serialInit();
}

bool Communication::sendData(uint8_t *data, int dataLength)
{
    TRACE;

    //serialPrintf()
    for (int i = 0; i < dataLength; i++)
    {
        serialPutChar(data[i]);
    }
    return true;
};

bool Communication::recvData(uint8_t *data, int * dataLength)
{
    TRACE;
    int recvDataSize = _getInputString(buffer, sizeof(buffer));

    int recvValue[10] = {0};
    int recvValuesCnt = 0;

#ifdef SERIAL_DEBUG
    serialPrintf("\nBuffer: %s\n", buffer);
#endif //SERIAL_DEBUG

    if (buffer[0] != 13 && buffer[0] != '\n')
    {
#ifdef SERIAL_DEBUG
        serialPrintf("Buffer is (probably) valid\n");
#endif //SERIAL_DEBUG

        char * delimiter = ";";
        char * pch = myStrtok (buffer, delimiter);
        while ((pch != NULL) && (recvValuesCnt < 10))
        {
#ifdef SERIAL_DEBUG
            serialPrintf("part%d: %s\n", recvValuesCnt, pch);
#endif //SERIAL_DEBUG
            recvValue[recvValuesCnt++] = myAtoi(pch);
            pch = myStrtok ((char*)NULL, delimiter);
        }
    }

    if (recvValuesCnt != 3)
    {
        return false;
    }

    uint8_t ACK[] = "ACK\n";

    this->sendData(ACK, sizeof(ACK));


    for (int i = 0; i <= recvValuesCnt; i++)
    {
        if (recvValue[i] < 0)
        {
            recvValue[i] = 0;
        }
        if (recvValue[i] > stepperMaxValue)
        {
            recvValue[i] = stepperMaxValue;
        }
    }

    sDatabasePacket setting = {{0}};

    setting.data[0].enable = true;
    setting.data[1].enable = true;
    setting.data[2].enable = true;
    setting.data[3].enable = false;

    setting.data[0].value = recvValue[0];
    setting.data[1].value = recvValue[1];
    setting.data[2].value = recvValue[2];

    myMemcpy(data, &setting, sizeof(setting));
    *dataLength = sizeof(setting);

    return true;
}
