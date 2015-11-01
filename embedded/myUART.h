/*
 * myUART.h
 *
 *  Created on: Oct 3, 2015
 *      Author: dulimon
 */

#ifndef MYUART_H_
#define MYUART_H_

#define UART0_CTL *((volatile unsigned long*)  0x4000C030)
#define UART0_IBRD *((volatile unsigned long*) 0x4000C024)
#define UART0_FBRD *((volatile unsigned long*) 0x4000C028)
#define UART0_LCRH *((volatile unsigned long*) 0x4000C02C)
#define UART0_CC *((volatile unsigned long*)   0x4000CFC8)
#define UART0_DATA *((volatile unsigned long*) 0x4000C000)

#define UART4_CTL *((volatile unsigned long*)  0x40010030)
#define UART4_IBRD *((volatile unsigned long*) 0x40010024)
#define UART4_FBRD *((volatile unsigned long*) 0x40010028)
#define UART4_LCRH *((volatile unsigned long*) 0x4001002C)
#define UART4_CC *((volatile unsigned long*)   0x40010FC8)
#define UART4_DATA *((volatile unsigned long*) 0x40010000)
#define UART4_FR *((volatile unsigned long*) 0x40010018)

#define UART4_BUSY (UART4_FR & 0x00000020>>5)

void UART_init();
void UART_write_byte(const char data);
void UART_write(const char data[],int size);
void UART_writeln(const char data[],int size);
void UART_write_minimal(const char data[],int size);
#endif /* MYUART_H_ */
