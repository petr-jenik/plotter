/*
 * plotterGui.h
 *
 *  Created on: Jan 20, 2016
 *      Author: apollo
 */

#ifndef GUI_H_
#define GUI_H_

#include <iostream>

using namespace std;

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
//#include <math.h>

//#include <unistd.h>

#include "global.h"

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

class Gui
{
private:
    static position windowSize;

public:
    static position getWindowSize(void);
    static void setWindowSize(position size);
    static void drawLine(position start, position end);
    static void clear(void);
    static void glSelectColor(eColor color);

    static void registerIdleFunction( void (* callback)( void ));
    static void registerDisplayFunction( void (* callback)( void ));
    static void registerTimerFunction( unsigned int time, void (* callback)( int ), int value);
    //static void registerUpdateCallback( void (* callback)( void ));
    static void registerMouseCallback( void (*callback)(int button, int state, int x, int y));
    static void registerKeyboardCallback( void (* callback)( unsigned char, int, int ));

    static void guiInit(int argc, char** argv);
    static void flush(void);
    static void forceRedraw(void);
    static void guiMainLoop(void);

    static void drawLinesStart();
    static void drawLines(position pos);
    static void drawLinesEnd();
};
#endif /* GUI_H_ */
