#include "myGPIO.h"
#include "myI2C.h"
#include "mySysTick.h"

void GPIO_init(){
		
		//Activate Port A and E
		volatile unsigned int delay = 0;
		PORT_CLK |= 0x00000035;
		delay = PORT_CLK;
	
	
		//UART 0 and 4 Enable
		RCGC_UART 	|= 0x00000011; 		// 1. Enable UART Module 0
		delay = RCGC_UART;
	
		//UART0
		UART0_CTL 	&= ~0x00000001;			//Disable UART
		UART0_IBRD	=		43;							//Integer Part of Baud rate
		UART0_FBRD	=		26;							//Fractional Part of Baud
		UART0_LCRH	=		0x00000070;			//1 Stop, No parity, 8bit
		//UART0_CC 		=	 	0x0;						//System Clk
		UART0_CTL 	|=  0x00000001;			//Enable UART
		
		//uart4
		UART4_CTL 	&= ~0x00000001;			//Disable UART
		UART4_IBRD	=		43;							//Integer Part of Baud rate
		UART4_FBRD	=		26;							//Fractional Part of Baud
		UART4_LCRH	=		0x00000070;			//1 Stop, No parity, 8bit
		//UART4_CC 		=	 	0x0;						//System Clk
		UART4_CTL 	|=  0x00000001;			//Enable UART
	
	
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
	
	
		//PA5 
		PORTA_DIR &= ~0x00000020;
		PORTA_AFSEL &= ~0x0020;
		PORTA_DEN |= 0x00000020;
	
		//PE4
		PORTE_DIR |= 0x00000010;
		PORTE_AFSEL &= ~0x10;
		PORTE_DEN |= 0x00000010;
	
		//PF1 - sw1
		PORTF_DIR |= 0x00000006;
		PORTF_AFSEL &= ~0x16;
		PORTF_DEN |= 0x00000016;
		PORTF_PUR |= 0x00000010;
		
		PORTF_IS &= ~0x00000010; 	//edge sensitive
		PORTF_BE &= ~0x00000010;		//clear Both enable, setting means EV has no effect
		PORTF_EV &= ~0x00000010;	//falling edge
		PORTF_IM |= 0x00000010;		//Mask
		
		//Enable interrupts on Port F
		INT_EN  |= 0x40000000;
		//INT_PRI &= ~0x00E00000;
}

//ISR
void GPIOPortF_Handler(){
	volatile unsigned int x;
		if(((PORTF_DATA & 0x00000004)>>2)==0){
			PORTF_DATA |= 0x00000004;
			
			//I2C_Write_slv(SLV_ADDR_GYRO,CTRL1,0xF);
			I2C_init_IMU();
			
			ST_RELOAD |= 0x00FFFFFF;
			//GetData(0x0F);			
			PORTF_DATA &= ~0x00000004;
		}else{
			PORTF_DATA &= ~0x00000004;
		}
		
		PORTF_CR  |= 0x00000010;
}

