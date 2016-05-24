#include "myGPIO.h"
#include "myI2C.h"
#include "mySysTick.h"
#include "IMUReader.h"

void GPIO_init(){
		
		//Activate Port A and E
		volatile unsigned int delay = 0;
		PORT_CLK |= 0x00000035;
		delay = PORT_CLK;

/*
		//PA5 
		PORTA_DIR &= ~0x00000020;
		PORTA_AFSEL &= ~0x0020;
		PORTA_DEN |= 0x00000020;
	
		//PE4
		PORTE_DIR |= 0x00000010;
		PORTE_AFSEL &= ~0x10;
		PORTE_DEN |= 0x00000010;
*/

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
			IMU_init();
			
			//ST_RELOAD |= 0x000FFFFF;
			ST_RELOAD |= 0x00000FFF;
			//GetData(0x0F);			
			PORTF_DATA &= ~0x00000004;
		}else{
			PORTF_DATA &= ~0x00000004;
		}
		
		PORTF_CR  |= 0x00000010;
}

