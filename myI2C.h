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
#define I2C_EXECUTE			 			 0x1
#define I2C_NEXECUTE			 		 0x9

//Accelorometer and Gyro 
#define SLV_ADDR_ACC 0x1D
#define SLV_ADDR_GYRO 0x6B
#define CTRL0_addr	0x1F
#define CTRL1_addr	0x20
#define CTRL2_addr	0x21
#define CTRL3_addr	0x22
#define CTRL4_addr	0x23
#define CTRL5_addr	0x24
#define CTRL6_addr	0x25
#define CTRL7_addr	0x26

#define CTRL0  0x000   //for normal mode, filters by-passed
#define CTRL1  0x4F   //update after read, and all axes of acceleration enabled at 25Hz
#define CTRL2  0xC1   //50Hz anti-alias, +/- 2g, no self-test, (SPI 3-wire)
#define CTRL3  0x00   //No INT1 actions
#define CTRL4  0x08   //accelerometer data ready on INT2.
#define CTRL5  0x0E   //No temperature, low-res magnetic, 25Hz, latch interrupt on INT2
#define CTRL6  0x00   //+/-2gauss sensitivity.
#define CTRL7  0x20   //normal high-pass acceleration filter, no Temp, magnetic always on, continuous 

void I2C_init(void);


void I2C_setReadSlv(unsigned int slvAddr,unsigned int read);
void I2C_setData(unsigned int val, unsigned int autoIncrAddr);
unsigned int I2C_CheckError(void);
void I2C_waitTillDone(void);
void I2C_waitTillIdle(void);
void I2C_Execute(unsigned char cmd);
void I2C_sendRecieveWithStop(int stop);
void I2C_readSlvChunk(const unsigned int slvAddr, const unsigned int slvReg,const unsigned int noRegs);
void I2C_readSlv(const unsigned int slvAddr, const unsigned int slvReg);
void I2C_writeSlv(const unsigned int slvAddr, const unsigned int slvReg, const unsigned int data);

void I2C_init_IMU(void);

//ISR
void I2C0_Handler(void);

#endif
