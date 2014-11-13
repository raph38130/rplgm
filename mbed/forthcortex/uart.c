/*
 * Serial.h
 *
 *  Created on: Dec 4, 2010
 *      Author: dejagerd
 */



#include <LPC17xx.h>


/**
 * Changes the configuration of the current port.
 * @param config Chooses the configuration for this port
 * @param baudrate Baudrate to use on this port
 * @return True if successful, FALSE if failed
 */
int SerialInit(uint32_t baudrate) {
	uint32_t Fdiv;
	uint32_t pclkdiv, pclk;
	uint32_t UART;

		UART = (uint32_t) LPC_UART0;

	pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
	switch (pclkdiv) {
	case 0x00:
	default:
		pclk = SystemCoreClock / 4;
		break;
	case 0x01:
		pclk = SystemCoreClock;
		break;
	case 0x02:
		pclk = SystemCoreClock / 2;
		break;
	case 0x03:
		pclk = SystemCoreClock / 8;
	}
	Fdiv = (pclk / 16) / baudrate; /*baud rate */

		LPC_PINCON->PINSEL0 &= ~((3 << 4) | (3 << 6));
		LPC_PINCON->PINSEL0 |= ((1 << 4) | (1 << 6)); /* RxD0 is P0.3 and TxD0 is P0.2 */
		//NVIC_EnableIRQ(UART0_IRQn);
		
		((LPC_UART0_TypeDef*) UART)->LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
		((LPC_UART0_TypeDef*) UART)->DLM = Fdiv / 256;
		((LPC_UART0_TypeDef*) UART)->DLL = Fdiv % 256;
		((LPC_UART0_TypeDef*) UART)->LCR = 0x03; /* DLAB = 0 */
		((LPC_UART0_TypeDef*) UART)->FCR = 0x07; /* Enable and reset TX and RX FIFO. */

	return (-1);
}

int uart_putchar(char c) {

	if (c == '\n')
		uart_putchar('\r');
	LPC_UART0->THR = c;
	while (!(LPC_UART0->LSR & (1 << 6)))
		;//TSR && THR are not empty
	return 0;
}
int uart_puts(char* s) {
	while (*s) {
		uart_putchar(*s);
		s++;

	}
	return 0;
}


