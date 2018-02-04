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


void reader_init(void)
{
}

void reader_readAndProcessFile(std::string fileName)
{
	DBG("Read GCODE data from file " << fileName << std::endl); // prints !!!Hello World!!!

	std::ifstream fileStream(fileName.c_str());
	if (fileStream.is_open())
	{
		std::string line;
		while (std::getline(fileStream, line))
		{
			parser_update(line);
		}
		LOG("Reading finished");
		fileStream.close();
	}
	else
	{
		LOG("Unable to open file");
	}
}
