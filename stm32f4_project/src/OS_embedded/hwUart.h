#ifndef HW_UART_H_
#define HW_UART_H_

#include <cstdint>
/*
#define BAUDRATE              115200
#define TXPIN                 GPIO_PIN_6
#define RXPIN                 GPIO_PIN_7
#define DATAPORT              GPIOB
#define UART_PRIORITY         15
#define UART_RX_SUBPRIORITY   0
#define MAXCLISTRING          100 // Biggest string the user will type
*/

typedef struct
{
	uint32_t baudrate;
} UartConfig;

typedef enum
{
	cUART_FIRST,
	cUART1 = cUART_FIRST,
	cUART6,
	cUART_DIM
} eUart;

bool uartInit(eUart channel, UartConfig& config);

void uartSendChar(char sendChar, eUart channel);

void uartPrint(char *string, eUart channel);

typedef enum
{
	State_OK,
	State_ERROR
} RxState;

typedef void (*RxCallback)(uint8_t rxByte, RxState state);

void uartRegisterRxCallback(RxCallback callback, eUart channel);

#endif // HW_UART_H_
