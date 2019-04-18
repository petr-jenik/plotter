#include "hwUart.h"
#include <string.h> // needed for strlen

//#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
//#include "stm32f4xx_hal_dma.h"

//uint8_t rxByte = '\0'; // where we store that one character that just came in
//uint8_t myBuffer = '\0';

/*
enum { cBufferSize = 1024};
uint8_t ringBuffer[cBufferSize];
int dataStart = 0;
int dataEnd = 0;
int dataLen = 0;

int newDataReceived = 0;
*/

//UART_HandleTypeDef huart;
//UART_HandleTypeDef huart6;
//DMA_HandleTypeDef hdma_usart1_rx;

/*
static void _dmaInit()
{
	hdma_usart1_rx.Instance = DMA2_Stream2;
	hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_rx.Init.MemInc = DMA_MINC_DISABLE;
	hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
	hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	HAL_DMA_Init(&hdma_usart1_rx);

	__HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);

	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, UART_PRIORITY, UART_RX_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

	return;
}

static void _dmaStart()
{
	//Start DMA
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_UART_Receive_DMA(&huart1, &rxByte, sizeof(rxByte));
	return;
}
*/

const uint32_t UART1_TXPIN = GPIO_PIN_6;
const uint32_t UART1_RXPIN = GPIO_PIN_7;

const uint32_t UART6_TXPIN = GPIO_PIN_6;
const uint32_t UART6_RXPIN = GPIO_PIN_7;

UART_HandleTypeDef gaUartHandlers[cUART_DIM];

UART_HandleTypeDef* getUartHandler(eUart channel)
{
	if ((channel != cUART1) and (channel != cUART6))
	{
		return nullptr;
	}

	return &gaUartHandlers[channel];
}


bool uartInit(eUart channel, UartConfig& config)
{
	if ((channel != cUART1) and (channel != cUART6))
	{
		return false;
	}

	UART_HandleTypeDef* pUartHandler = getUartHandler(channel);
	GPIO_TypeDef * gpioPort = nullptr;

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;

	//Set up the UART
	pUartHandler->Init.BaudRate = config.baudrate;
	pUartHandler->Init.WordLength = UART_WORDLENGTH_8B;
	pUartHandler->Init.StopBits = UART_STOPBITS_1;
	pUartHandler->Init.Parity = UART_PARITY_NONE;
	pUartHandler->Init.Mode = UART_MODE_TX_RX;
	pUartHandler->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	pUartHandler->Init.OverSampling = UART_OVERSAMPLING_16;

	switch(channel)
	{
	case cUART1:
	{
		// Setup USART1
		__GPIOB_CLK_ENABLE();
		__USART1_CLK_ENABLE();
		GPIO_InitStruct.Pin = UART1_TXPIN | UART1_RXPIN;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		pUartHandler->Instance = USART1;
		gpioPort = GPIOB;
		break;
	}
	case cUART6:
	{
		// Setup USART6
		__GPIOC_CLK_ENABLE();
		__USART6_CLK_ENABLE();
		GPIO_InitStruct.Pin = UART6_TXPIN | UART6_RXPIN;
		GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
		pUartHandler->Instance = USART6;
		gpioPort = GPIOC;
		break;
	}
	default:
		return false;
		break;
	}


	HAL_GPIO_Init(gpioPort, &GPIO_InitStruct);
	HAL_UART_Init(pUartHandler);

	return true;

	/*
	// Setup USART6

	// Set up IO

	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
*/
	/*
	//Set up the UART
	huart6.Init.BaudRate = BAUDRATE;
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.Parity = UART_PARITY_NONE;
	huart6.Init.Mode = UART_MODE_TX_RX;
	huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart6.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart6);
*/
}

/*
	//Set up DMA interrupt
void DMA2_Stream2_IRQHandler(void)
{
    HAL_NVIC_ClearPendingIRQ(DMA2_Stream2_IRQn);
    HAL_DMA_IRQHandler(&hdma_usart1_rx);
}
*/
/*
int uartDataAvailable(void)
{
	return newDataReceived;
}*/
/*
char uartGetChar(void)
{
	while(!newDataReceived){};

	uint8_t byte = ringBuffer[dataStart++];
	if (dataStart == dataEnd)
	{
		dataEnd == 0;
		dataStart == 0;
	    newDataReceived = 0;
	}

	return byte;
}
*/
void uartSendChar(char sendChar, eUart channel)
{
	UART_HandleTypeDef* pUartHandler = getUartHandler(channel);

	if (pUartHandler != nullptr)
	{
		HAL_UART_Transmit(pUartHandler, (uint8_t*)&sendChar, 1, 5);
	}
}

void uartPrint(char *string, eUart channel)
{
	UART_HandleTypeDef* pUartHandler = getUartHandler(channel);

	if (pUartHandler != nullptr)
	{
		HAL_UART_Transmit(pUartHandler, (uint8_t*)string, strlen(string), 5);
	}
}

RxCallback gaRxCallbacks[cUART_DIM];
uint8_t gaRxDataBuffers[cUART_DIM];

IRQn_Type gaUartIrqTypes[] = {USART1_IRQn, USART6_IRQn};

void uartRegisterRxCallback(RxCallback callback, eUart channel)
{
	UART_HandleTypeDef* pUartHandler = getUartHandler(channel);

	if (pUartHandler != nullptr)
	{
		gaRxCallbacks[channel] = callback;
		HAL_UART_Receive_IT(pUartHandler, &gaRxDataBuffers[channel], 1 /* receive 1 byte only*/);

		IRQn_Type irqType = gaUartIrqTypes[channel];

		HAL_NVIC_SetPriority(irqType, 3, 0);
	    HAL_NVIC_EnableIRQ(irqType);
	}
}

/**
* @brief This function handles USART2 global interrupt.
*/


/**
  * @brief This function handles USART2 global interrupt.
  */
extern "C" void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

    HAL_NVIC_ClearPendingIRQ(USART6_IRQn);

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(getUartHandler(cUART1));
  /* USER CODE BEGIN USART2_IRQn 1 */

  // Enable RX interrupt
  HAL_UART_Receive_IT(getUartHandler(cUART1), &gaRxDataBuffers[cUART1], 1 /* receive 1 byte only*/);


  /* USER CODE END USART2_IRQn 1 */
}
//
//extern "C" void USART1_IRQHandler(void)
//{
//  /* USER CODE BEGIN USART2_IRQn 0 */
//    HAL_NVIC_ClearPendingIRQ(USART1_IRQn);
//
//  /* USER CODE END USART2_IRQn 0 */
//  HAL_UART_IRQHandler(getUartHandler(cUART1));
//  /* USER CODE BEGIN USART2_IRQn 1 */
//
//  /* USER CODE END USART2_IRQn 1 */
//}

/*
void EXTI0_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  {
	  // EXTI line interrupt detected
	  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
	  {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
		HAL_GPIO_EXTI_Callback(GPIO_PIN_0);
	  }
	}
}
*/

extern "C" void USART6_IRQHandler(void)
{
    HAL_NVIC_ClearPendingIRQ(USART6_IRQn);

    // Get UART handler
    UART_HandleTypeDef* pUartHandler = getUartHandler(cUART6);

    RxState state = State_OK;

    // Overrun error? Clear that bastard!
    if (__HAL_UART_GET_FLAG(pUartHandler, UART_FLAG_ORE) != RESET)
    {
    	__HAL_UART_CLEAR_OREFLAG(pUartHandler);

    	state = State_ERROR;
    }

    // Give me your data!
    uint32_t data = __HAL_UART_FLUSH_DRREGISTER(pUartHandler);

	// If callback registered?
	if (nullptr != gaRxCallbacks[cUART6])
	{
		// Call registered callback
		gaRxCallbacks[cUART6]((uint8_t)data, state);
	}

    // Enable interrupt
    /* Enable the UART Parity Error Interrupt */
    SET_BIT(pUartHandler->Instance->CR1, USART_CR1_PEIE);

    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    SET_BIT(pUartHandler->Instance->CR3, USART_CR3_EIE);

    /* Enable the UART Data Register not empty Interrupt */
     SET_BIT(pUartHandler->Instance->CR1, USART_CR1_RXNEIE);

    // Enable RX interrupt
    //HAL_UART_Receive_IT(getUartHandler(cUART6), &gaRxDataBuffers[cUART6], 1 /* receive 1 byte only*/);

  /* USER CODE END USART6_IRQn 1 */
}


extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *pUartHandler)
{
//#define __HAL_UART_FLUSH_DRREGISTER(__HANDLE__) ((__HANDLE__)->Instance->DR)
    //eUart channel = cUART_DIM - 1; // Something like invalid value
    //bool channelFound = false;

	if (pUartHandler->Instance == USART1)
	{
		// If callback registered?
		if (nullptr != gaRxCallbacks[cUART1])
		{
			// Call registered callback
			gaRxCallbacks[cUART1](gaRxDataBuffers[cUART1], State_OK);
		}
	}
}
    //uartPrint(&rxBuffer);
    //memcpy(&myBuffer, &rxBuffer, sizeof(rxBuffer));

    /*

    if (dataEnd < cBufferSize)
    {
        ringBuffer[dataEnd++] = rxByte;
    }

    newDataReceived = 1;

	// Echo (send back received character)
	uartSendChar(rxByte);
	*/


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* Peripheral clock enable */
//    __HAL_RCC_USART1_CLK_ENABLE();
//
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**USART2 GPIO Configuration
//    PA2     ------> USART2_TX
//    PA3     ------> USART2_RX
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }

}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }

}

