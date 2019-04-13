#include "Arduino.h"
#include "appLogic_main.h"

#include "log.h"

//The setup function is called once at startup of the sketch
void setup()
{
	TRACE; // Trace macro

	// Add your initialization code here
	appLogic_init();
}

// The loop function is called in an endless loop
void loop()
{
	TRACE; // Trace macro

	//Add your repeated code here
	appLogic_loop();
}
