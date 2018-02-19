#include <stdio.h>
#include <stdlib.h>
//#include "diag/Trace.h"

#include "Timer.h"

#include "hwGpio.h"

#include "mainApp.h"

#include "hwStepperPins.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 led blink sample (trace via NONE).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// Definitions visible only within this translation unit.

// ----- LED definitions ------------------------------------------------------

#warning "Assume a STM32F4-Discovery board, PC12-PC15, active high."


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

// ----- main() ---------------------------------------------------------------


//Gpio swInput1(getSwitchPins(0).limitSwitch1);
//Gpio swInput2(getSwitchPins(0).limitSwitch2);

int main()
{
    //swInput1.powerUp();
    //swInput2.powerUp();
    // At this stage the system clock should have already been configured
    // at high speed.
    //trace_printf("System clock: %u Hz\n", SystemCoreClock);

    Timer timer;
    timer.start ();

    // Perform all necessary initialisations for the LEDs.
    for (size_t i = 0; i < (sizeof(blinkLeds) / sizeof(blinkLeds[0])); ++i)
    {
        blinkLeds[i].powerUp ();
    }

    int i = 0;
    while(1)
    {
        /*
        if (true == swInput1.isOn())
        {
            blinkLeds[0].turnOn();
        }
        else
        {
            blinkLeds[0].turnOff();
        }

        if (true == swInput2.isOn())
        {
            blinkLeds[1].turnOn();
        }
        else
        {
            blinkLeds[1].turnOff();
        }
*/
        blinkLeds[i].toggle();
        i = (i + 1)%4;
        mainApp();
    }

/*
uint32_t seconds = 0;

for (size_t i = 0; i < (sizeof(blinkLeds) / sizeof(blinkLeds[0])); ++i)
{
blinkLeds[i].turnOn ();
}

// First second is long.
timer.sleep (Timer::FREQUENCY_HZ);

for (size_t i = 0; i < (sizeof(blinkLeds) / sizeof(blinkLeds[0])); ++i)
{
blinkLeds[i].turnOff ();
}

timer.sleep (BLINK_OFF_TICKS);

++seconds;
//trace_printf ("Second %u\n", seconds);

if ((sizeof(blinkLeds) / sizeof(blinkLeds[0])) > 1)
{
// Blink individual LEDs.
for (size_t i = 0; i < (sizeof(blinkLeds) / sizeof(blinkLeds[0])); ++i)
{
blinkLeds[i].turnOn ();
timer.sleep (BLINK_ON_TICKS);

blinkLeds[i].turnOff ();
timer.sleep (BLINK_OFF_TICKS);

++seconds;
//trace_printf ("Second %u\n", seconds);
}

// Blink binary.
while (1)
{
for (size_t l = 0; l < (sizeof(blinkLeds) / sizeof(blinkLeds[0]));
++l)
{
blinkLeds[l].toggle ();
if (blinkLeds[l].isOn ())
{
break;
}
}
timer.sleep (Timer::FREQUENCY_HZ);

++seconds;
//trace_printf ("Second %u\n", seconds);
}
// Infinite loop, never return.
}
else
{
while (1)
{
blinkLeds[0].turnOn ();
timer.sleep (BLINK_ON_TICKS);

blinkLeds[0].turnOff ();
timer.sleep (BLINK_OFF_TICKS);

++seconds;
//trace_printf ("Second %u\n", seconds);
}
// Infinite loop, never return.
}
*/
}

// ----------------------------------------------------------------------------
