/*
 * parser.h
 *
 *  Created on: 11. 2. 2017
 *      Author: apollo
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "global.h"
#include "stddef.h"
#include "stdint.h"
#include <string>

typedef char * cmdPart;

const int maxNumberOfGCmdParts = 10;

typedef struct
{
	cmdPart parts[maxNumberOfGCmdParts];
	int32_t length;
	bool used;
} command;

const int dataBufferSize = 100;

typedef void(*cbf_moveTo_t)(position finalPosition, float movementSpeed, float extrudeLength);

class path_parser
{
	//line_function = None;
   //move_function = None;
    //cubic_bezier_function = None;
    //quadratic_bezier_function = None;

private:
	char dataBuffer[dataBufferSize];

    position offset_position;
    float zoom;
    command cmdData;

    bool hasAttribute(command data, std::string attributes);
    bool getValue(command data, char name, float &refValue);

    cbf_moveTo_t cbf_moveTo;

public:
    path_parser(float _zoom = 1, float _offset_x = 0, float _offset_y = 0, float _offset_z = 0)
	:offset_position{ _offset_x, _offset_y, _offset_z}
    ,zoom{_zoom}
	,cmdData{{NULL}, 0, false}
	,cbf_moveTo{NULL}
	{};

    ~path_parser(){};

    void registerMoveToCallback(cbf_moveTo_t callbackF)
    {
    	cbf_moveTo = callbackF;
    }

    void set_offset_position(position offset);
    void set_zoom(float _zoom);
    //void newData(char* pData, size_t dataLength);
    void newData(const char* pData, const size_t dataLength);
    //void newData(std::string newLine);
    void update();
};


#endif /* PARSER_H_ */
