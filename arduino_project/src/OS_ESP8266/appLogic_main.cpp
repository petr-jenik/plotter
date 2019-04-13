#include "Timer.h"
#include "hwGpio.h"
#include "hwStepperPins.h"

#include "reader_main.h"
#include "parser_main.h"
#include "movementControl_main.h"
#include "stepperControl_main.h"

#include "global.h"

#include "servo.h"

// ----- LED definitions ------------------------------------------------------
#define LED_PORT         (3)
#define LED_PIN_GREEN    (12)
#define LED_PIN_ORANGE   (13)
#define LED_PIN_RED      (14)
#define LED_PIN_BLUE     (15)
#define LED_ACTIVE_LOW          (false)

Gpio blinkLeds[] =
{
	{{ LED_PORT, LED_PIN_GREEN,  LED_ACTIVE_LOW, eGPIO_Mode_OUTPUT_PP,  eGPIO_Pull_No }},
    {{ LED_PORT, LED_PIN_ORANGE, LED_ACTIVE_LOW, eGPIO_Mode_OUTPUT_PP,  eGPIO_Pull_No }},
    {{ LED_PORT, LED_PIN_RED,    LED_ACTIVE_LOW, eGPIO_Mode_OUTPUT_PP,  eGPIO_Pull_No }},
    {{ LED_PORT, LED_PIN_BLUE,   LED_ACTIVE_LOW, eGPIO_Mode_OUTPUT_PP,  eGPIO_Pull_No }}
};
// ----- main() ---------------------------------------------------------------

Timer timer;

void heartbeat(void)
{
	blinkLeds[0].toggle();
}

void heartbeat_mainApp(void)
{
	blinkLeds[1].toggle();
}



void atLimitSwitch(int i)
{
	if (i == 0)
	{
		blinkLeds[2].turnOn();
	}
	else
	{
		blinkLeds[3].turnOn();
	}
}


void LEDs_init(void)
{
	TRACE; // Trace macro
	// Perform all necessary initialisations for the LEDs.
	for (size_t i = 0; i < (sizeof(blinkLeds) / sizeof(blinkLeds[0])); ++i)
	{
		blinkLeds[i].powerUp ();
	}
}



void LED_loop(void)
{
    timer.start ();
    LEDs_init();

	// Button init
	Gpio userButton({eGPIO_PORT_A, 0, false, eGPIO_Mode_Input, eGPIO_Pull_No });
	userButton.powerUp();

	while(1)
	{
		if (userButton.isOn())
		{
			static int idx = 0;
			blinkLeds[idx].turnOff();
			idx = (idx + 1) % 4;
			blinkLeds[idx].turnOn();

			Timer::sleep(100);
		}
	}

}

void systemInit()
{
	TRACE; // Trace macro

    timer.start ();

    // Heartbeat LED initialization
    LEDs_init();
}


void appInit()
{
	TRACE; // Trace macro
	// Order of initialization is important!
	stepperControl_init();
	movementControl_init();
	parser_init();
	reader_init();
}

void appLogic_init()
{
	TRACE; // Trace macro

	LOG("Application initialization");
	/* System init */
	systemInit();

	// User button init
	Gpio userButton({eGPIO_PORT_A, 0, false, eGPIO_Mode_Input, eGPIO_Pull_No });
	userButton.powerUp();

	/*
	// Wait for user button press
	while(false == userButton.isOn())
	{
		Timer::sleep(100);
		heartbeat_mainApp();
	}*/

	/* App init */
	appInit();

	Timer::sleep(1000);

	/*
	// Wait for user button press
	while(false == userButton.isOn())
	{
		Timer::sleep(100);
		heartbeat_mainApp();
	}*/
}

//#define DEBUG_LOOP

void appLogic_loop()
{
	TRACE; // Trace macro

	/* Start main app */

	/*
	{
		PlotterServo usedServos[] =
		{
				{0}, {1}, {2}
		};

		while(1)
		{
			ServoSetting setting;
			setting.enable = true;

			setting.angle = 0;
			usedServos[0].OnUpdate(setting);

			setting.angle = 180;
			usedServos[1].OnUpdate(setting);

			usedServos[0].OnMove();
			usedServos[1].OnMove();

			Timer::sleep(100);
		}
	}
	*/


#ifdef DEBUG_LOOP
	movementControl_showDemo();
#else
	reader_readAndProcess();
#endif // #ifdef DEBUG_LOOP

/*	while(1)
	{
		LOG("PROGRAM END");
		Timer::sleep(100);
	}
*/
}


// ----------------------------------------------------------------------------
