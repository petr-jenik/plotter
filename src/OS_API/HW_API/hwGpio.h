/*
 * hwGpio.h
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#ifndef HWAPI_HWGPIO_H_
#define HWAPI_HWGPIO_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include <stdint.h>

const unsigned int gpioPinUndef = (unsigned int)- 1;
const unsigned int gpioPortUndef = (unsigned int)- 1;

typedef enum
{
    eGPIO_PORT_A = 0,
    eGPIO_PORT_B,
    eGPIO_PORT_C,
    eGPIO_PORT_D,
    eGPIO_PORT_E,
    eGPIO_PORT_H = 7,
    eGPIO_PORT_COUNT
} eGPIO_Port;

typedef enum
{
    eGPIO_Mode_Input = GPIO_MODE_INPUT,
    eGPIO_Mode_OUTPUT_PP = GPIO_MODE_OUTPUT_PP,
    eGPIO_Mode_OUTPUT_OD = GPIO_MODE_OUTPUT_OD,
    eGPIO_Mode_AF_PP = GPIO_MODE_AF_PP,
    eGPIO_Mode_AF_OD = GPIO_MODE_AF_OD,
} eGPIO_Mode;

typedef enum
{
    eGPIO_Pull_No = GPIO_NOPULL,
    eGPIO_Pull_Up = GPIO_PULLUP,
    eGPIO_Pull_Down = GPIO_PULLDOWN,
} eGPIO_Pull;

typedef enum
{
    ePin_Dir = 0,
    ePin_Step,
    ePin_Reset,
    ePin_Sleep,
    ePin_Enable,
    ePin_LimSw1,
    ePin_LimSw2,

    ePin_undef,
} ePinType;

typedef struct
{
    uint32_t port;
    uint32_t pin;
    bool activeLow;
    eGPIO_Mode mode;
    eGPIO_Pull pull;
} GpioDesc_t;

class Gpio
{
public:
  Gpio (GpioDesc_t gpioDesc);

  void
  powerUp ();

  void
  turnOn ();

  void
  turnOff ();

  void
  toggle ();

  bool
  isOn ();

  bool isValid();

  bool operator==(Gpio &A);
private:
    bool valid;
    eGPIO_Pull pull;
    eGPIO_Mode mode;
    unsigned int fPortNumber;
    unsigned int fBitNumber;
    unsigned int fBitMask;
    bool fIsActiveLow;
};

#endif /* HWAPI_HWGPIO_H_ */
