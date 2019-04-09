/*
 * parser.cpp
 *
 *  Created on: 5. 2. 2017
 *      Author: apollo
 */

#include <cstdint>
#include <cstdlib>
#include <string.h>
#include "parser.h"

static const int STRCMP_MATCH = 0;


void path_parser::set_offset_position(position offset)
{
	LOG("Set offset position");
	offset_position = offset;
}


void path_parser::set_zoom(float _zoom)
{
	LOG("Set zoom");
	zoom = _zoom;
}


void path_parser::newData(const char* pData, const size_t dataLength)
{
	memset(dataBuffer, 0, sizeof(dataBuffer));
	memset(cmdData.parts, 0, sizeof(cmdData.parts));

	size_t sizeToCopy = MIN(dataLength, sizeof(dataBuffer));

	memcpy(dataBuffer, pData, sizeToCopy);

	// Check for comments in new line data
	if (dataBuffer[0] == ';')
	{
		cmdData.length = 0;
		cmdData.used = false;
		return;
	}

	const char delimiters[] = " ";
	char *token;

	/* get the first token */
	token = strtok(dataBuffer, delimiters);

	int i = 0;

	if (token != NULL)
	{
		cmdData.parts[i++] = token;
	}

	/* walk through other tokens */
	while( token != NULL )
	{
		token = strtok(NULL, delimiters);
		if (token != NULL)
		{
			cmdData.parts[i++] = token;
		}
	}

	cmdData.length = i;
	cmdData.used = (i) ? true : false;
}

/*
 *
 *  G0 : Rapid linear Move
    G1 : Linear Move

Usage
    G0 Xnnn Ynnn Znnn Ennn Fnnn Snnn
    G1 Xnnn Ynnn Znnn Ennn Fnnn Snnn
Parameters
    Not all parameters need to be used, but at least one has to be used
    Xnnn The position to move to on the X axis
    Ynnn The position to move to on the Y axis
    Znnn The position to move to on the Z axis
    Ennn The amount to extrude between the starting point and ending point
    Fnnn The feedrate per minute of the move between the starting point and ending point (if supplied)
    Snnn Flag to check if an endstop was hit (S1 to check, S0 to ignore, S2 see note, default is S0)
 *
 *
 */


void path_parser::update()
{
	if ((cmdData.length == 0) || (cmdData.used == false))
	{
		return;
	}

	static float x = 0;         // Xnnn - The position to move to on the X axis
	static float y = 0;         // Ynnn - The position to move to on the Y axis
	static float z = 0;         // Znnn - The position to move to on the Z axis
	static float extrude = 0;   // Ennn - The amount to extrude between the starting point and ending point
	static float moveSpeed = 0; // Fnnn - The feedrate per minute of the move between the starting point and ending point (if supplied)

	if ((strcmp(cmdData.parts[0], "G1") == STRCMP_MATCH) ||
        (strcmp(cmdData.parts[0], "G0") == STRCMP_MATCH))
	{
		// Every G0/G1 has to have E parameter
		if (this->hasAttribute(cmdData, "E") == false)
		{
			extrude = 0;
			LOG("Invalid attributes - at least E is requested for G0/G1\n");
			//LOG(cmdData);
			return;
		}
		else
		{
			this->getValue(cmdData, 'E', extrude);
		}

		// Read optional F parameter and save it
		if (hasAttribute(cmdData, "F"))
		{
			getValue(cmdData, 'F', moveSpeed);
		}

		// Read optional Z parameter and save it
		if (hasAttribute(cmdData, "Z"))
		{
			getValue(cmdData, 'Z', z);
		}

		// Read optional parameters XY
		if (this->hasAttribute(cmdData, "XY") == false)
		{
			LOG("Invalid attributes - at least XY are requested for G0/G1\n");
			//LOG(cmdData);
			return;
		}

		// Optional parameters XY found
		this->getValue(cmdData, 'X', x);
		this->getValue(cmdData, 'Y', y);

		position newPosition;
		newPosition.x = this->zoom * (x - this->offset_position.x);
		newPosition.y = this->zoom * (y - this->offset_position.y);
		newPosition.z = this->zoom * (z - this->offset_position.z);

		if (cbf_moveTo != NULL)
		{
			cbf_moveTo(newPosition, moveSpeed, extrude);
		}
		else
		{
			LOG("Invalid moveTo callback function!!\n");
		}

		// Clear command
		cmdData.used = false;
		cmdData.length = 0;
	}
}

bool path_parser::getValue(command data, char name, float &refValue)
{
	for (int i = 0; i < data.length; i++)
	{
		if (data.parts[i][0] == name)
		{
			refValue = atof(&data.parts[i][1]);
			return true;
		}
	}
	return false;
}

//bool path_parser::hasAttribute(command data, std::string attributes);
bool path_parser::hasAttribute(command data, std::string attributes)
{
	//char * test = attributes;
	auto length = strlen(attributes.c_str());
	for (uint32_t i = 0; i < length; i++)
	{
		float unusedValue;
		if (getValue(data, attributes.c_str()[i], unusedValue) == false)
		{
			return false;
		}
	}
	return true;
}

/*
    def get_bbox(self, path):
        print "Get bounding box"
        self.bbox = bounding_box();
        line_function_backup = self.line_function;
        move_function_backup = self.move_function;
        cubic_bezier_function_backup = self.cubic_bezier_function;
        quadratic_bezier_function_backup = self.quadratic_bezier_function

        self.line_function = self.bbox.draw_line
        self.move_function = self.bbox.draw_move
        self.cubic_bezier_function = self.bbox.draw_cubic_bezier
        self.quadratic_bezier_function = self.bbox.draw_quadratic_bezier

        self.parse_path(path)

        self.line_function = line_function_backup
        self.move_function = move_function_backup
        self.cubic_bezier_function = cubic_bezier_function_backup
        self.quadratic_bezier_function = quadratic_bezier_function_backup

        return self.bbox.get_bounding_box()

    def set_gui_update_function(self, gui_update):
        self.gui_update = gui_update;

    def transform(self, position):
        position -= self.offset_position
        x = position.real * self.zoom
        y = position.imag * self.zoom
        return x + 1j * y

    def set_line_function(self, function):
        print "Set line function"
        self.line_function = function;

    def set_move_z_function(self, function):
        print "Set line function"
        self.move_z_function = function;

    def set_move_function(self, function):
        print "Set move function"
        self.move_function = function;

    def set_cubic_bezier_function(self, function):
        print "Set cubic Bezier function"
        self.cubic_bezier_function = function;

    def set_quadratic_bezier_function(self, function):
        print "Set quadratic Bezier function"
        self.quadratic_bezier_function = function;

    def _tokenize_path(self, path):
        for x in self.COMMAND_RE.split(path):
            if x in self.COMMANDS:
                yield x
            for token in self.FLOAT_RE.findall(x):
                yield token

    def draw_to_box(self, path, target_bbox1, target_bbox2):
        print "Draw to box"

        self.set_offset_position(0)
        self.set_zoom(1)
        bbox1, bbox2 = self.get_bbox(path);

        current_width = bbox2.real - bbox1.real;
        current_heigth = bbox2.imag - bbox1.imag;

        target_width = target_bbox2.real - target_bbox1.real;
        target_heigth = target_bbox2.imag - target_bbox1.imag;
        width_zoom = target_width / current_width
        heigth_zoom = target_heigth / current_heigth
        zoom = min(width_zoom, heigth_zoom)

        self.set_zoom(zoom)
        self.set_offset_position(bbox1 - (target_bbox1 / zoom))
        print "Parse path data and draw"
        self.parse_path(path)
*/
