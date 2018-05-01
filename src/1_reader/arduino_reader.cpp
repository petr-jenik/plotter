/*
 * fake_reader.cpp
 *
 *  Created on: 24. 2. 2018
 *      Author: apollo
 */

#include "log.h"
#include <string>
#include <FS.h> // File system
#include "parser_main.h"

void listOfFiles()
{
	String str = "";
	Dir dir = SPIFFS.openDir("/");
	while (dir.next())
	{
	    str += dir.fileName();
	    str += " / ";
	    str += dir.fileSize();
	    str += "\r\n";
	}
	LOG(str.c_str());
}


void reader_readAndProcess()
{
	TRACE; // Trace macro

	const char * fileName = "/sample.gcode";

 	DBG("Read GCODE data from file " << fileName << std::endl); // prints !!!Hello World!!!
 	File file = SPIFFS.open(fileName, "r");

 	if (file.available())
 	{
		LOG("Input file open: OK");
 		while (file.available())
 		{
 			String line = file.readStringUntil('\n');
			parser_update(line.c_str(), line.length());
 		}
 		LOG("Reading finished");
 		file.close();
 	}
 	else
 	{
 		LOG("Unable to open file: " << fileName);
 		listOfFiles();
 	}
}

void reader_init(void)
{
	TRACE; // Trace macro

	DBG("Very basic Spiffs example, writing 10 lines to SPIFFS filesystem, and then read them back");

	SPIFFS.begin();
	// Next lines have to be done ONLY ONCE!!!!!When SPIFFS is formatted ONCE you can comment these lines out!!
	//Serial.println("Please wait 30 secs for SPIFFS to be formatted");
	SPIFFS.format();
	DBG("Spiffs formatted");
};

