/*
 * plotterGui.h
 *
 *  Created on: Jan 20, 2016
 *      Author: apollo
 */

#ifndef GUI_H_
#define GUI_H_

#include <iostream>

#include "gui_types.h"

using namespace std;

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
//#include <math.h>

//#include <unistd.h>

class Gui
{
private:
    static position windowSize;

public:
    static position getWindowSize(void);
    static void setWindowSize(position size);
    static void clear(void);
    static void glSelectColor(eColor color);
    static void registerUpdateCallback( void (* callback)( void ));
    static void registerMouseCallback( void (*callback)(int button, int state, int x, int y));
    static void guiInit(int argc, char** argv);
    static void forceRedraw(void);
    static void guiMainLoop(void);
};

#endif /* GUI_H_ */
