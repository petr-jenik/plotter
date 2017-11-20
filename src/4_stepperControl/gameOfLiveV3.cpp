#define GLUT_DISABLE_ATEXIT_HACK

#define CYCLE_SLEEP 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <iostream>

#include <cstdlib>
#include <iostream>
#include <ctime>

#include <unistd.h>

#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end

//#include <Windows.h>

using namespace std;

enum {eWidth = 200,
      eHeight = 200
};


class Cell
{
    bool status;
    bool newStatus;

    int keepAliveData[10];
    int keepAliveCount;

    int bringBackToLifeData[10];
    int bringBackToLifeCount;

private:

    bool isInArray(int * array, int arrayLength, int value)
	{
    	bool retval = false;

    	for (int i = 0; i < arrayLength; i++)
    	{
    		if (array[i] == value)
    		{
    			retval = true;
    			break;
    		}
    	}
    	return retval;
    }

public:
    Cell()
    {
    	// Bludiste
    	int tmpArray[] = {2,3};//{1,2,3,4,5};
    	int tmpArray2[] = {3};

    	//int tmpArray[] = {2, 3};
		//int tmpArray2[] = {3};


    	memcpy(this->keepAliveData, tmpArray, sizeof(tmpArray));
    	this->keepAliveCount = sizeof(tmpArray)/sizeof(tmpArray[0]);

    	memcpy(this->bringBackToLifeData, tmpArray2, sizeof(tmpArray2));
    	this->bringBackToLifeCount = sizeof(tmpArray2)/sizeof(tmpArray2[0]);

        this->status = false;
        this->newStatus = false;
    }

    bool isAlive(void)
    {
        return this->status;
    }

    void kill(void)
    {
        this->newStatus = false;
    }

    void bringBackToLife(void)
    {
        this->newStatus = true;
    }

    // n - numberOfNeighbors
    void evolve(int n)
    {

    	if (true == this->status)
    	{
    		if (isInArray(this->keepAliveData, this->keepAliveCount, n))
    		{
                this->bringBackToLife();
    		}
    		else
    		{
    			this->kill();
    		}
    	}
    	else
    	{
    		if (isInArray(this->bringBackToLifeData, this->bringBackToLifeCount, n))
    		{
                this->bringBackToLife();
    		}
    	}

    	/*
        if (n == 3)
        {
            map.getCell(x,y)->bringBackToLife();
        }

        if (n == 2 &&     map.getCell(x,y)->isAlive())
        {
            map.getCell(x,y)->bringBackToLife();
        }

        if ((n < 2) || (n > 3))
        {
            map.getCell(x,y)->kill();
        }
        */
    }

    void update(void)
    {
    	this->status = this->newStatus;
    }
};

//Cell map[eWidth][eHeight];

class Map
{
    int height;
    int width;

    Cell ** cellArray;

public:
    Map(int width, int height)
    {
        this->width = width;
        this->height = height;

        this->cellArray = new Cell * [height];
        for (int i = 0; i < height; i++)
        {
            this->cellArray[i] = new Cell [width];
        }
    }

    int getWidth(void)
    {
        return this->width;
    }

    int getHeight(void)
    {
        return this->height;
    }

    Cell * getCell(int x, int y)
    {
        return &this->cellArray[y][x];
    }

    void update(void)
    {
        for (int y = 0; y < this->height; y++)
        {
            for (int x = 0; x < this->width; x++)
            {
                this->cellArray[y][x].update();
            }
        }
    }

    int getNumberOfCellNeighbors(int x, int y)
    {
        int n = 0;

        for (int yPos = y - 1; yPos <= (y + 1); yPos++)
        {
            for (int xPos = x - 1; xPos <= (x + 1); xPos++)
            {
                int yPosTmp = (yPos + this->height) % this->height;
                int xPosTmp = (xPos + this->width) % this->width;

                if (this->cellArray[yPosTmp][xPosTmp].isAlive())
                {
                    n++;
                }
            }
        }

        if(this->cellArray[y][x].isAlive())
        {
            n--;
        }
        return n;
    }
};

Map map(eWidth, eHeight);

void drawLines(void)
{
    /* clear all pixels */

    glClear (GL_COLOR_BUFFER_BIT);

    glLineWidth(1);
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);

    for (int y = 1; y < eHeight; y++)
    {
            float yVal = (float)y / eHeight;
            glVertex3f(0, yVal, 0.0);
            glVertex3f(1, yVal, 0.0);
    }

    for (int x = 0; x < eWidth; x++)
    {
            float xVal = (float)x / eWidth;
            glVertex3f(xVal, 0, 0.0);
            glVertex3f(xVal, 1, 0.0);
    }

    glEnd();
}


void evolve(void)
{
    for (int y = 0; y < map.getHeight(); y++)
    {
        for (int x = 0; x < map.getWidth(); x++)
        {
            int n = map.getNumberOfCellNeighbors(x,y);

            map.getCell(x,y)->evolve(n);
        }
    }

    map.update();
}

void drawBox(int x,int y)
{
    float xSize = (float)1 / eHeight;
    float ySize = (float)1 / eHeight;

    glColor3f (0, 0, 0);
    glBegin(GL_POLYGON);
    glVertex3f (x * xSize, y * ySize, 0.0);
    glVertex3f ((x + 1) * xSize, y * ySize, 0.0);
    glVertex3f ((x + 1) * xSize, (y+1) * ySize, 0.0);
    glVertex3f (x * xSize, (y+1) * ySize, 0.0);
    glEnd();

    /*
    glColor3f (1, 1, 1);
    glRasterPos2f(x * xSize, y * ySize); // location to start printing text

    int n = map.getNumberOfCellNeighbors(x,y);

    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, '0' + n); // Print a character on the
    glEnd();
	*/

}

void show(void)
{

    //glClear (GL_COLOR_BUFFER_BIT);

    drawLines();

    for (int y = 0; y < eHeight; y++)
    {
        for (int x = 0; x < eWidth; x++)
        {
            if(map.getCell(x,y)->isAlive() == true)
            {
                drawBox(x,y);
            }
        }
    }



    glFlush ();
}

void init (void)
{
    /* select clearing (background) color */
    glClearColor (1.0, 1.0, 1.0, 0.0);

    /* initialize viewing values */
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void gameUpdate(void)
{
    show();
    evolve();

    //100
    usleep(CYCLE_SLEEP*1000);
}

void randomAtPosition(int xPos, int yPos)
{
	static int start = 0;
	if (start == 0)
	{
		std::srand(std::time(0)); // use current time as seed for random generator
		start++;
	}

    int randomVar = 0;

    for(int y = yPos - 10 ; y < yPos + 10; y++)
    {
        for(int x = xPos - 10; x <  xPos + 10; x++)
        {
            randomVar = std::rand();
            if (randomVar % 2)
            {
            	int tmpX = (x + eWidth) % eWidth;
            	int tmpY = (y + eHeight) % eHeight;

                map.getCell(tmpX, tmpY)->bringBackToLife();
            }
        }
    }

    map.update();

}



int main(int argc, char** argv)
{
	randomAtPosition(100, 100);
	randomAtPosition(30, 30);
	randomAtPosition(0, 0);

    glutInit(&argc, argv);

    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize (600, 600);

    glutInitWindowPosition (100, 100);

    glutCreateWindow ("hello");

    init ();

    //cout << "Hello" << endl;

    //glutDisplayFunc(show);
    glutIdleFunc(gameUpdate);

    //glutDisplayFunc(show);

    glutMainLoop();

    return 0;
}
