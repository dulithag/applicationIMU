#include "myI2C.h"
#include "myGPIO.h"
#include "myUART.h"

void I2C_init(){
	
		//Enable Port B
		volatile unsigned int delay = 0;
		PORT_CLK |= 0x00000002;
		delay = PORT_CLK;
	
		//Enable I2C
		RCGC_I2C 	|= 0x00000001; 		// 1. Enable I2C Module 0
		delay = RCGC_I2C;
	
	
		//PB2 PB3
		PORTB_DIR 	|= 0x0000000C;
		PORTB_AFSEL |= 0x0000000C;	  	
		//PORTB_PCTL	|= 0x00003300; //this is set by default to i2c
		PORTB_PUR		|= 0x0000000C; 	 //pull up register for S	
		PORTB_DEN 	|= 0x0000000C;
		PORTB_ODR		|= 0x00000008;	//SDA set to open Drain Operation	
	
		I2C0_MCR		|= 0x00000010;	//Select Master or Slave
		I2C0_MTPR		 = 0x00000039;	//I2C Clk 

}




//SlvAddr + Read is 1, Write is 0
void I2C_setReadSlv(unsigned int slvAddr, unsigned int read){
	unsigned int x = read & 0x01;
	I2C0_MSA = (slvAddr<<1)+ x;
}

void I2C_setData(unsigned int val, unsigned int autoIncrAddr){
	I2C0_MDR = (val | (autoIncrAddr<<7));
}

//Error - 1; NoError - 0
unsigned int I2C_CheckError(){
	volatile unsigned int status 			= I2C0_MCS;
	volatile unsigned int NoAckError 			= (I2C0_MCS& 0x02)>>1;
	volatile unsigned int ClkTimeoutError = (I2C0_MCS& 0x80)>>7;
	volatile unsigned int NoAddrAckError = (I2C0_MCS& 0x04)>>2;
	return NoAckError | ClkTimeoutError | NoAckError ;
}

//wait till i2c bus is idle
void I2C_waitTillDone(){
		while(((I2C0_MCS & 0x40)>>6)==1);
}

void I2C_waitTillIdle(){
		while(((I2C0_MCS & 0x1))==1);
}



void I2C_Execute(unsigned char cmd){	
			I2C0_MCS = cmd;
}

void I2C_sendRecieveWithStop(int stop){	
		if(stop==1){
			I2C0_MCS = 0x7;	//Start Transmit/Recieve, Stop
		}else{
			I2C0_MCS = 0x3;	//Start Transmit/Recieve
		}
}

void I2C_readSlvChunk(const unsigned int slvAddr, const unsigned int startReg, const unsigned int noRegs,  char array[]){
		
		volatile char data_a = 0;
		int i=0;
	
		//Waits for i2c
		I2C_waitTillDone();
	
		//Send Register
		I2C_setReadSlv(slvAddr,0);
		I2C_setData(startReg, 1);	
		I2C_sendRecieveWithStop(0);
		//I2C_Execute(I2C_START_EXECUTE);
		I2C_waitTillIdle();
	
		I2C_setReadSlv(slvAddr,1);
		I2C_Execute(0x0B);
		for(i=0; i<noRegs; i++){
			I2C_waitTillIdle();
			if(I2C_CheckError()==0){
				data_a = I2C0_MDR;
				array[i] = data_a;
				//UART4_DATA = x_data_a;
			}
			I2C_Execute((i==noRegs-1)?I2C_EXECUTE_STOP:I2C_EXECUTE);
		}

		
}

void I2C_readSlv(const unsigned int slvAddr, const unsigned int slvReg){
	
    volatile unsigned int x_data_a = 0;	
		unsigned int Error = 0;
	
		//Waits for i2c
		I2C_waitTillDone();
			
		//Send Register
		I2C_setReadSlv(slvAddr,0);
		I2C_setData(slvReg,0);							
		I2C_sendRecieveWithStop(0);
		I2C_waitTillIdle();
		Error = I2C_CheckError();
		if(Error)
			UART4_DATA = 'E';
		
		//Receive Data
		I2C_setReadSlv(slvAddr,1);
		I2C_sendRecieveWithStop(1);
		I2C_waitTillDone();
		if(I2C_CheckError()==0){
			x_data_a = I2C0_MDR;
			UART4_DATA = x_data_a;
		}
}

void I2C_writeSlv(const unsigned int slvAddr, const unsigned int slvReg, const unsigned int data){
	
    volatile unsigned int x_data_a = 0;	
		unsigned int Error = 0;
	
		//Waits for i2c
		I2C_waitTillDone();
			
		//Send START, Register
		I2C_setReadSlv(slvAddr,0);
		I2C_setData(slvReg,0);							
		I2C_Execute(I2C_START_EXECUTE);
		I2C_waitTillIdle();
		Error = I2C_CheckError();
		if(Error)
			UART4_DATA = 'E';
		
		//Send Data, STOP
		I2C_setData(data,0);							
		I2C_Execute(I2C_EXECUTE_STOP);
	
		
		I2C_waitTillDone();
		
		Error = I2C_CheckError();
		if(Error)
			UART4_DATA = 'E';
		
}

void I2C0_Handler(){
}
