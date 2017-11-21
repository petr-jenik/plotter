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

class thread_guard
{
   std::thread& m_thread;
   thread_guard( const thread_guard& );
   thread_guard& operator=
      ( const thread_guard& );
public:
   thread_guard( std::thread& t )
      : m_thread(t)
   {   }
   ~thread_guard()
   {
      if( m_thread.joinable() )
         m_thread.join();
   }
};


void systemInit()
{
	reader_init();
	parser_init();
	movementControl_init();
	stepperControl_init();
}

// std::thread t (&non_member_function);
// thread_guard(t)
// do_seomething_and_throw()
// Pri destrukci thread_guardu se na thread zavola join, tim se ukonci
// Pote prestane existovat objekt t (bez thread_guardu by objekt t prestal existovat, ale thread by bezel dal)

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
#ifndef NO_GUI
	/* Create GUI loop before an initialization of the rest of the system */
	std::thread thr_GUI				(gui_loop);
#endif // #ifdef NO_GUI

	/* App init */
	systemInit();

	unsigned int pocetProcesoru = std::thread::hardware_concurrency();

	std::cout << "Pocet procesoru: " << pocetProcesoru << std::endl;

	std::vector<std::thread> threadPool;

	//safe_queue<moveCommand> queue_parser_movementControl;
	//safe_queue<armCommand> queue_movementControl_motorControl;
	//safe_queue<stepperCommand> queue_motorControl_GUI;

	//std::thread thr_movementControl	(movementControl_loop, 	std::ref(queue_parser_movementControl),			std::ref(queue_movementControl_motorControl)	);
	//std::thread thr_motorControl	(motorControl_loop, 	std::ref(queue_movementControl_motorControl),	std::ref(queue_motorControl_GUI)				);

#ifdef DEBUG_LOOP
	std::thread thr_debug_loop			(debug_loop);
	threadPool.push_back(move(thr_debug_loop));
#else
	safe_queue<string> queue_reader_parser;

	std::thread thr_reader			(reader_loop,			std::ref(queue_reader_parser));
	std::thread thr_parser			(parser_loop, 			std::ref(queue_reader_parser));//,					std::ref(queue_parser_movementControl)			);

	threadPool.push_back(move(thr_reader));
	threadPool.push_back(move(thr_parser));
#endif // #ifdef DEBUG_LOOP

	//threadPool.push_back(move(thr_movementControl));
	//threadPool.push_back(move(thr_motorControl));
#ifndef NO_GUI
	threadPool.push_back(move(thr_GUI));
#endif // NO_GUI
	std::cout << "Main task..." << std::endl;

	for (auto it = threadPool.begin(); it != threadPool.end(); it++)
	{
		(*it).join();
	}

	std::cout << "PROGRAM END" << std::endl;

	/*
	for( std::thread& t : threadPool )
	{
		t.join();
	}
	*/

	return 0;
}
