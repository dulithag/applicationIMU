#ifndef __MYI2C__
#define __MYI2C__


#define RCGC_I2C *((volatile unsigned long*) 0x400FE620)

#define I2C0_MSA 	*((volatile unsigned long*) 	0x40020000)	
#define I2C0_MCR 	*((volatile unsigned long*) 	0x40020020)
#define I2C0_MTPR *((volatile unsigned long*) 	0x4002000C)
#define I2C0_MDR 	*((volatile unsigned long*) 	0x40020008)
#define I2C0_MCS 	*((volatile unsigned long*) 	0x40020004)	

//Commands for send/receive
#define I2C_START_EXECUTE_STOP 0x7
#define I2C_START_EXECUTE 		 0x3
#define I2C_EXECUTE_STOP 			 0x5
#define I2C_STOP					 		 0x4
#define I2C_NEXECUTE			 			 0x1
#define I2C_EXECUTE			 		 0x9



void I2C_init(void);


void I2C_setReadSlv(unsigned int slvAddr,unsigned int read);
void I2C_setData(unsigned int val, unsigned int autoIncrAddr);
unsigned int I2C_CheckError(void);
void I2C_waitTillDone(void);
void I2C_waitTillIdle(void);
void I2C_Execute(unsigned char cmd);
void I2C_sendRecieveWithStop(int stop);
void I2C_readSlvChunk(const unsigned int slvAddr, const unsigned int slvReg,const unsigned int noRegs,char array[]);
void I2C_readSlv(const unsigned int slvAddr, const unsigned int slvReg);
void I2C_writeSlv(const unsigned int slvAddr, const unsigned int slvReg, const unsigned int data);


//ISR
void I2C0_Handler(void);

#endif
