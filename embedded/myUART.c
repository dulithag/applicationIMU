/*
 * myUART.c
 *
 *  Created on: Oct 3, 2015
 *      Author: dulimon
 */
#include "myGPIO.h"
#include "myUART.h"


void UART_init(){
		//UART 0 and 4 Enable
		volatile int delay = 0;
		RCGC_UART 	|= 0x00000011; 		// 1. Enable UART Module 0
		delay = RCGC_UART;

		//UART0
		UART0_CTL 	&= 		~0x00000001;				//Disable UART
		UART0_IBRD	=		43;							//Integer Part of Baud rate
		UART0_FBRD	=		26;							//Fractional Part of Baud
		UART0_LCRH	=		0x00000070;					//1 Stop, No parity, 8bit
		//UART0_CC 	=	 	0x0;						//System Clk
		UART0_CTL 	|=  	0x00000001;					//Enable UART

		//uart4
		UART4_CTL 	&= 		~0x00000001;				//Disable UART
		UART4_IBRD	=		43;							//Integer Part of Baud rate
		UART4_FBRD	=		26;							//Fractional Part of Baud
		UART4_LCRH	=		0x00000070;					//1 Stop, No parity, 8bit
		//UART4_CC 	=	 	0x0;						//System Clk
		UART4_CTL 	|=  	0x00000001;					//Enable UART


		//PA 0-1 as UART
		PORTA_AFSEL |= 0x00000003; 		// 3. Set AFSEL for pin
		PORTA_DEN 	|= 0x00000003;
		PORTA_PCTL 	 = (PORTA_PCTL&0xFFFFFF00)+0x00000011;					// 5. Configure GPIOPCTL to assign UART singnals to pins
		PORTA_AMSEL &= ~0x03;

		//PC 4-5 as UART
		PORTC_AFSEL |= 0x00000030; 		// 3. Set AFSEL for pin
		PORTC_DEN 	|= 0x00000030;
		PORTC_PCTL 	 = (PORTC_PCTL&0xFF00FFFF)+0x00110000;					// 5. Configure GPIOPCTL to assign UART singnals to pins
		PORTC_AMSEL &= ~0x03;
}

void UART_write_byte(const char data){
	while(UART4_BUSY != 0);
	UART4_DATA = data;
}

void UART_write(const char data[],int size){
	int i = 0;
	volatile int delay = 0;
	for(i=0; i<size; i++){
		while(delay<10000){delay++;}
		UART_write_byte(data[i]);

	}
}

void UART_write_minimal(const char data[],int size){
	int i=0;
	while(data[i]=='0'){i++;}
	if(i>=size)
		UART_write_byte('0');
	else{
		for(; i<size; i++){
			UART_write_byte(data[i]);
		}
	}
}

void UART_writeln(const char data[],int size){
	UART_write(data,size);
	UART_write_byte('\r');
	UART_write_byte('\n');
}


