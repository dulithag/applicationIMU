#include "mySysTick.h"
#include "myI2C.h"
#include "myGPIO.h"

extern int sCounter;
extern int sLightOn;

void SysTick_Handler(){
		//PORTF_DATA |= 0x00000002;
		sCounter++;
		if(sCounter>1){
				if(sLightOn==1){
						sLightOn = 0;
						PORTF_DATA |= 0x00000002;
					
						//I2C_readSlv(SLV_ADDR_ACC,0x0F);	//Whoami 49
						//I2C_readSlv(SLV_ADDR_GYRO,0x0F);	//whoami D7
						
						I2C_readSlv(SLV_ADDR_ACC,0x29);
						I2C_readSlv(SLV_ADDR_ACC,0x28);
					
				}else{
						sLightOn = 1;
						PORTF_DATA &= ~0x00000002;
				}
				
				sCounter = 0;
		}
}

void sysTickInit(){
		//ST_RELOAD |= 0x00FFFFFF;
		ST_RELOAD |= 0x00;
		ST_CURRENT = 0x0;
		ST_CTRL = 0x7;
}
