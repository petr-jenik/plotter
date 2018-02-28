//============================================================================
// Name        : threads_example.cpp
// Author      : Apollo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <vector>

#include "reader_main.h"
#include "parser_main.h"
#include "movementControl_main.h"
#include "stepperControl_main.h"

#include "global.h"
#include "my_gui.h"

using namespace std;

void systemInit()
{
	// Order of initialisation is important!
	stepperControl_init();
	movementControl_init();
	parser_init();
	reader_init();
}


/*
 * List of tasks:
 *
 * 	1. reader - reads line of data from serial (or from file) - add data to parser
 * 	2. parser - parses data - create commands from them (move, line, etc)
 *  3. movement_control - reads commands and compute movements
 *  4. motor_control - controls steppers according to commands (get them to required position)
 *  5. (optional) GUI (plot actual state)
 *
 */

int main(int argc, char** argv)
{
	unsigned int pocetProcesoru = std::thread::hardware_concurrency();
	std::cout << "Pocet procesoru: " << pocetProcesoru << std::endl;

	/* Create GUI loop before an initialization of the rest of the system */
	std::thread thread_GUI(gui_loop);

	/* App init */
	systemInit();

//#define DEBUG_LOOP

	/* Start main app */
#ifdef DEBUG_LOOP
	movementControl_showDemo();
#else
	//std::string fileName = "sample.gcode";
	reader_readAndProcess();
#endif // #ifdef DEBUG_LOOP

	thread_GUI.join();

	std::cout << "PROGRAM END" << std::endl;

	return 0;
}
