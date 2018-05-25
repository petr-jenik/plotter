/*
 * newMain.cpp
 *
 *  Created on: 23. 5. 2018
 *      Author: apollo
 */

#include "Timer.h"
#include "global.h"
#include "Serial.h"

// ----- main() ---------------------------------------------------------------

Timer timer;

void initialize_hardware(void)
{
  // Initialise the HAL Library; it must be the first function
  // to be executed before the call of any HAL function.
  HAL_Init();

  // Enable HSE Oscillator and activate PLL with HSE as source
  //SystemClock_Config();

  // Call the CSMSIS system clock routine to store the clock frequency
  // in the SystemCoreClock global RAM location.
  SystemCoreClockUpdate();
}

void systemInit()
{
	initialize_hardware();

	LOG("System clock: %u Hz\n" <<  SystemCoreClock);

    timer.start ();
}


void appInit()
{
}


int main(int argc, char** argv)
{
	/* System init */
	systemInit();

	// TODO remove this - start
	Timer::sleep(1000);

	Serial serial();

	while(1)
	{
		//serial.writeX(serial.readX());
	}

	return 0;
}


// ----------------------------------------------------------------------------
