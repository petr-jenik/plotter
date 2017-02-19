/*
 * reader.cpp
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#include <iostream>
#include <fstream>
#include <cstdio>
#include <errno.h>

#include "app_threads.h"

//using namespace std;


void reader_loop(safe_queue<std::string> &queueOutput)
{
	std::string fileName = "sample.gcode";

	std::cout << "Read GCODE data from file " << fileName << std::endl; // prints !!!Hello World!!!

	//std::ifstream infile("thefile.txt");
	std::ifstream fileStream(fileName.c_str());
	if (fileStream.is_open())
	{
		std::cout << "Input file opened" << std::endl;
		std::string line;
		while (std::getline(fileStream, line))
		{
			std::cout << "Thread: " << __FUNCTION__ << std::endl;
			std::cout << "Data:" << line << std::endl;
			queueOutput.send(line);
			auto delay = std::chrono::milliseconds(10);
			std::this_thread::sleep_for(delay);
		}
		fileStream.close();
	}
	else
	{
		std::cout << "Unable to open file" << std::endl;
	}
}
