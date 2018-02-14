//============================================================================
// Name        : threads_example.cpp
// Author      : Apollo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
//#include "log.h"
#include <thread>
//#include <mutex>
//#include <condition_variable>
//#include <future>
//#include <chrono>
//#include <atomic>
#include <vector>
//#include <deque>

//#include "blocking_queue.h"

#include "app_threads.h"

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
//#define NO_GUI
#ifndef NO_GUI
	/* Create GUI loop before an initialization of the rest of the system */
	std::thread thr_GUI				(gui_loop);
#endif // #ifdef NO_GUI

	/* App init */
	systemInit();

	unsigned int pocetProcesoru = std::thread::hardware_concurrency();
	std::cout << "Pocet procesoru: " << pocetProcesoru << std::endl;
	std::vector<std::thread> threadPool;

#define DEBUG_LOOP

#ifdef DEBUG_LOOP
	std::thread thr_debug_loop			(debug_loop);
	threadPool.push_back(move(thr_debug_loop));
#else

	std::string fileName = "sample.gcode";

	std::thread thr_mainProcess			(reader_readAndProcessFile,			std::ref(fileName));
	threadPool.push_back(move(thr_mainProcess));
#endif // #ifdef DEBUG_LOOP

#ifndef NO_GUI
	threadPool.push_back(move(thr_GUI));
#endif // NO_GUI
	std::cout << "Main task..." << std::endl;

	for (auto it = threadPool.begin(); it != threadPool.end(); it++)
	{
		(*it).join();
	}

	std::cout << "PROGRAM END" << std::endl;

	return 0;
}
