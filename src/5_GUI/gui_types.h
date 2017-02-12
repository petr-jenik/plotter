/*
 * gui_types.h
 *
 *  Created on: 3. 2. 2017
 *      Author: apollo
 */

#ifndef GUI_GUI_TYPES_H_
#define GUI_GUI_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    eColor_blue,
    eColor_red,
    eColor_black,
    eColor_green,

	eColor_COUNT,

	// Unused colors
    eColor_white
} eColor;

typedef struct
{
    float x;
    float y;
    float z;
} position;

typedef enum
{
	eLine
} guiCommandType;

typedef struct
{
	guiCommandType type;
	position pos1;
	position pos2;
} guiCommand;

#ifdef __cplusplus
}
#endif


#endif /* GUI_GUI_TYPES_H_ */
