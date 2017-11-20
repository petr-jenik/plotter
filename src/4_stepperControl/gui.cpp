/*
 * plotterGui.c
 *
 *  Created on: Jan 20, 2016
 *      Author: apollo
 */
#include <iostream>

using namespace std;

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <gui.h>

#include "global.h"

position Gui::windowSize = {800, 800};

void Gui::clear(void)
{
    glClear (GL_COLOR_BUFFER_BIT);
}

position Gui::getWindowSize(void)
{
    return Gui::windowSize;
};

void Gui::setWindowSize(position size)
{
    Gui::windowSize = size;
};

void Gui::drawLine(position start, position end)
{
    glLineWidth(1);
    //glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2f((float)start.x /  Gui::windowSize.x, 1 - ((float)start.y / Gui::windowSize.y));
    glVertex2f((float)end.x / Gui::windowSize.x, 1 - ((float)end.y / Gui::windowSize.y));
    glEnd();
}

void Gui::glSelectColor(eColor color)
{
    switch(color)
    {
    case eColor_blue:
        glColor3f (0, 0, 1);
        break;
    case eColor_red:
        glColor3f (1, 0, 0);
        break;
    case eColor_black:
        glColor3f (0, 0, 0);
        break;
    case eColor_white:
        glColor3f (1, 1, 1);
        break;
    case eColor_green:
        glColor3f (0, 1, 0);
        break;
    default:
        //Black
        glColor3f (0, 0, 0);
        break;
    }
}

void Gui::registerUpdateCallback( void (* callback)( void ))
{
    glutIdleFunc(callback);
}

void Gui::registerMouseCallback( void (*callback)(int button, int state, int x, int y))
{
    glutMouseFunc(callback);
}

static int windowId = -1;

void Gui::guiInit(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (Gui::getWindowSize().x, Gui::getWindowSize().y);
    glutInitWindowPosition (100, 100);
    windowId = glutCreateWindow ("hello");

    /* select clearing (background) color */
    glClearColor (1.0, 1.0, 1.0, 0.0);

    /* initialize viewing values */
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    //glOrtho(0.0, 00, 0.0, 0.0, 0, 1.0);
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

}

void Gui::forceRedraw(void)
{
	glutPostRedisplay();
}

void reg()
{

}


void Gui::guiMainLoop(void)
{
    glutMainLoop();
}
