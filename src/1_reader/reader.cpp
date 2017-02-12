/*
 * reader.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include "app_threads.h"

using namespace std;

void reader_loop(safe_queue<string> &queueOutput)
{
	string gcode_example[] = {"M 0,0",   \
							"X 100,0",   \
							"X 100,100", \
							"X 0,100",   \
							"X 0,0",     \
							"KONEC"};
	std::cout << "sizeof(gcode_example): " << sizeof(gcode_example) << std::endl;
	std::cout << "sizeof(gcode_example[0]): " << sizeof(gcode_example[0]) << std::endl;

	for (int i = 0; i < (sizeof(gcode_example)/sizeof(gcode_example[0])); i++)
	{
		std::cout << "Thread: " << __FUNCTION__ << std::endl;
		queueOutput.send(gcode_example[i]);
		auto delay = std::chrono::milliseconds(100);
		std::this_thread::sleep_for(delay);
	}
}

