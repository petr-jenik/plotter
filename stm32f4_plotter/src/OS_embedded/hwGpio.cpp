/*
 * hwGpio.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: apollo
 */

#include "hwGpio.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define BLINK_GPIOx(_N)       ((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))
#define BLINK_PIN_MASK(_N)    (1 << (_N))
#define BLINK_RCC_MASKx(_N)   (RCC_AHB1ENR_GPIOAEN << (_N))


GpioDesc_t undefPin = {gpioPinUndef, gpioPortUndef, false, eGPIO_Mode_Input, eGPIO_Pull_Down};
// ----------------------------------------------------------------------------

Gpio::Gpio(GpioDesc_t gpioDesc)
{
    if ((gpioDesc.pin == gpioPinUndef) || (gpioDesc.port == gpioPortUndef))
    {
        valid = false;
        return;
    }

    valid = true;
    pull = gpioDesc.pull;
    mode = gpioDesc.mode;
    fPortNumber = gpioDesc.port;
    fBitNumber = gpioDesc.pin;
    fIsActiveLow = gpioDesc.activeLow;
    fBitMask = BLINK_PIN_MASK(fBitNumber);
}

void
Gpio::powerUp ()
{
    if (false == valid)
    {
        return;
    }
    // Enable GPIO Peripheral clock
    RCC->AHB1ENR |= BLINK_RCC_MASKx(fPortNumber);

    GPIO_InitTypeDef GPIO_InitStructure;

    // Configure pin in output push/pull mode
    GPIO_InitStructure.Pin = fBitMask;
    GPIO_InitStructure.Mode = mode;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Pull = pull;
    HAL_GPIO_Init (BLINK_GPIOx(fPortNumber), &GPIO_InitStructure);

    // Start with led turned off
    turnOff ();
}

void
Gpio::turnOn ()
{
    if (false == valid)
    {
        return;
    }

    if (fIsActiveLow)
    {
      BLINK_GPIOx(fPortNumber)->BSRR = BLINK_PIN_MASK(fBitNumber + 16);
    }
  else
    {
      BLINK_GPIOx(fPortNumber)->BSRR = fBitMask;
    }
}

void
Gpio::turnOff ()
{
    if (false == valid)
    {
        return;
    }

    if (fIsActiveLow)
    {
      BLINK_GPIOx(fPortNumber)->BSRR = fBitMask;
    }
  else
    {
      BLINK_GPIOx(fPortNumber)->BSRR = BLINK_PIN_MASK(fBitNumber + 16);
    }
}

void
Gpio::toggle ()
{
    if (false == valid)
    {
        return;
    }

    if (BLINK_GPIOx(fPortNumber)->IDR & fBitMask)
    {
      BLINK_GPIOx(fPortNumber)->ODR &= ~fBitMask;
    }
  else
    {
      BLINK_GPIOx(fPortNumber)->ODR |= fBitMask;
    }
}

bool
Gpio::isOn ()
{
    if (false == valid)
    {
        return false;
    }

    bool isHigh;

    if (fIsActiveLow)
    {
      isHigh = ((BLINK_GPIOx(fPortNumber)->IDR & fBitMask) == 0) ? true: false;
    }
    else
    {
      isHigh = ((BLINK_GPIOx(fPortNumber)->IDR & fBitMask) != 0) ? true: false;
    }

    return isHigh;
}

bool Gpio::isValid()
{
    return this->valid;
}

// ----------------------------------------------------------------------------

