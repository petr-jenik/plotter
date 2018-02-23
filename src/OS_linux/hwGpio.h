/*
 * hwGpio.h
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#ifndef HWAPI_HWGPIO_H_
#define HWAPI_HWGPIO_H_

#include <iostream>
//#include "myGlobal.h"
//#include "stm32f4xx.h"
//#include "stm32f4xx_hal.h"

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
    eGPIO_Mode_Input     = 0,
    eGPIO_Mode_OUTPUT_PP = 1,
    eGPIO_Mode_OUTPUT_OD = 2,
    eGPIO_Mode_AF_PP     = 3,
    eGPIO_Mode_AF_OD     = 4,
} eGPIO_Mode;

typedef enum
{
    eGPIO_Pull_No   = 0,
    eGPIO_Pull_Up   = 1,
    eGPIO_Pull_Down = 2,
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
    int devId; // ID of an owner of an instance of the Gpio object
    ePinType pin;

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
private:
    bool valid;
    int devId;
    ePinType pin;

    std::ios_base::Init mInitializer;
};

extern GpioDesc_t undefPin;

#endif /* HWAPI_HWGPIO_H_ */
