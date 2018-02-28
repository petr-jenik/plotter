/*
 * stm32f4xx_hal.h
 *
 *  Created on: 25. 2. 2018
 *      Author: apollo
 */

#ifndef OS_LINUX_STM32F4XX_HAL_H_
#define OS_LINUX_STM32F4XX_HAL_H_


enum
{
	GPIO_MODE_INPUT = 0,
	GPIO_MODE_OUTPUT_PP,
	GPIO_MODE_OUTPUT_OD,
	GPIO_MODE_AF_PP,
	GPIO_MODE_AF_OD
};

enum
{
	GPIO_NOPULL = 0,
	GPIO_PULLUP,
	GPIO_PULLDOWN
};




#endif /* OS_LINUX_STM32F4XX_HAL_H_ */
