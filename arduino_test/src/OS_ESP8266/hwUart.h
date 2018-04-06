#ifndef HW_UART_H_
#define HW_UART_H_

#ifdef __cplusplus
	extern "C" {
#endif

void uartInit();
void uartSendChar(char sendChar);
void uartSendString(const char * pString);
char uartGetChar(void);


#ifdef __cplusplus
	}
#endif

#endif // HW_UART_H_
