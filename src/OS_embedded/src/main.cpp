#include <stdio.h>
#include <stdlib.h>

#include "Timer.h"
#include "hwGpio.h"
#include "hwStepperPins.h"

#include "app_threads.h"

#define BLINK_PORT_NUMBER         (3)
#define BLINK_PIN_NUMBER_GREEN    (12)
#define BLINK_PIN_NUMBER_ORANGE   (13)
#define BLINK_PIN_NUMBER_RED      (14)
#define BLINK_PIN_NUMBER_BLUE     (15)
#define BLINK_ACTIVE_LOW          (false)

GpioDesc_t ledsDesc[4] =
  {
    { BLINK_PORT_NUMBER, BLINK_PIN_NUMBER_GREEN,  BLINK_ACTIVE_LOW, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No },
    { BLINK_PORT_NUMBER, BLINK_PIN_NUMBER_ORANGE, BLINK_ACTIVE_LOW, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No },
    { BLINK_PORT_NUMBER, BLINK_PIN_NUMBER_RED,    BLINK_ACTIVE_LOW, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No },
    { BLINK_PORT_NUMBER, BLINK_PIN_NUMBER_BLUE,   BLINK_ACTIVE_LOW, eGPIO_Mode_OUTPUT_PP, eGPIO_Pull_No },
  };

Gpio blinkLeds[] =
{
        {ledsDesc[0]},
        {ledsDesc[1]},
        {ledsDesc[2]},
        {ledsDesc[3]},
};
using namespace std;

void systemInit()
{
	LOG("System clock: %u Hz\n" <<  SystemCoreClock);

    Timer timer;
    timer.start ();

    // Perform all necessary initialisations for the LEDs.
    for (size_t i = 0; i < (sizeof(blinkLeds) / sizeof(blinkLeds[0])); ++i)
    {
        blinkLeds[i].powerUp ();
    }

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
	/* App init */
	systemInit();

//#define DEBUG_LOOP

	/* Start main app */
#ifdef DEBUG_LOOP
	debug_loop();
#else
	reader_readAndProcessFile(fileName);
#endif // #ifdef DEBUG_LOOP

	LOG("PROGRAM END");
	return 0;
}
