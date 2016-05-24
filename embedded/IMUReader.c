/*
 * IMUReader.c
 *
 *  Created on: Oct 3, 2015
 *      Author: dulimon
 */
#include "IMUReader.h"
#include "myI2C.h"
#include "myUART.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define SENSIVITY 16675 //this is (faceup - facedown)/2 - not converted to v/g

/*----------------------------------------
 * 			Utility Functions
 *---------------------------------------*/

bool flipbits(int16_t* value){
	int msb = ((*value & 0x8000) >> 15);

	if(*value < 0){
		*value = ((*value & 0x8000) - (*value & ~0x8000)  );
	}

	return (bool)msb;
}

void myitoa(int16_t value, char buffer[5]){
	int digit = 0;
	int i = 0;
	for(i=4; i>=0; i--){
		digit = value % 10;
		value = value / 10;
		buffer[i] = "0123456789"[digit];
	}
}

void printUART_decimal(struct unit_t datapoint){
	char bufferx[5];
	char buffery[5];
	char bufferz[5];
	volatile bool negative = false;
	int16_t tdata = 0;
	volatile int delay = 0;

	//x
	tdata = datapoint.x;
	negative = flipbits(&tdata);
	myitoa(tdata,bufferx);
	if(negative)
		UART_write_byte('-');
	UART_write_minimal(bufferx,5);
	UART_write_byte(' ');

	//y
	tdata = datapoint.y;
	negative = flipbits(&tdata);
	myitoa(tdata,buffery);
	if(negative)
		UART_write_byte('-');
	UART_write_minimal(buffery,5);
	UART_write_byte(' ');

	//z
	while(delay<10000){delay++;}		//delay stupid UART is slow, need some sync with endpoint
	tdata = datapoint.z;
	negative = flipbits(&tdata);
	myitoa(tdata,bufferz);
	if(negative)
		UART_write_byte('-');
	UART_write_minimal(bufferz,5);
	UART_write_byte('\n');
}

void printUART_hex(struct unit_t datapoint){

	unsigned char data[6];

	data[0] = (datapoint.x >> 8);
	data[1] = (datapoint.x & 0x0000FFFF);

	data[2] = (datapoint.y >> 8);
	data[3] = (datapoint.y & 0x0000FFFF);

	data[4] = (datapoint.z >> 8);
	data[5] = (datapoint.z & 0x0000FFFF);

	UART_write((const char*)data,6);
	UART_write_byte(0x5C);
	UART_write_byte(0x6E);
}

void readUnit(uint8_t slvAddr,uint8_t startAddr, struct unit_t* datapoint ){
	char data[6];
	I2C_readSlvChunk(slvAddr,startAddr,6,data);

/*
	//TEST CODE
	data[0] = 0xA1;
	data[1] = 0x70;
	data[2] = 0xA2;
	data[3] = 0x80;
	data[4] = 0xA3;
	data[5] = 0x90;
	//UART_write(data,6);
*/

	int i = 0;
	for(i=0; i<6; i+=2){
		int16_t low = data[i];
		int16_t high = data[i+1];
		int16_t concat = (high<<8) + low;
		if(i==0)
			datapoint->x = concat;
		else if(i==2)
			datapoint->y = concat;
		else
			datapoint->z = concat;
	}
}

/*---------------------------------------
 * 				Accelerometer
 *---------------------------------------*/


void accelerometer_init(){

	I2C_writeSlv(SLV_ADDR_GYRO,CTRL1_addr,0x0F);

	//Data update at 50Hz, enable X, Y, Z
	I2C_writeSlv(SLV_ADDR_ACC,CTRL0_addr,ACC_CTRL0);
	I2C_writeSlv(SLV_ADDR_ACC,CTRL1_addr,ACC_CTRL1);
	I2C_writeSlv(SLV_ADDR_ACC,CTRL2_addr,ACC_CTRL2);
	I2C_writeSlv(SLV_ADDR_ACC,CTRL3_addr,ACC_CTRL3);
	I2C_writeSlv(SLV_ADDR_ACC,CTRL4_addr,ACC_CTRL4);
	I2C_writeSlv(SLV_ADDR_ACC,CTRL5_addr,ACC_CTRL5);
	I2C_writeSlv(SLV_ADDR_ACC,CTRL6_addr,ACC_CTRL6);
	I2C_writeSlv(SLV_ADDR_ACC,CTRL7_addr,ACC_CTRL7);

	char *data = (char*)malloc(8);
	I2C_readSlvChunk(SLV_ADDR_ACC,CTRL0_addr,8,data);
	//UART_write(data,8);
}

void accelerometer_dataProcess(struct unit_t* datapoint){

	int16_t datapointx = datapoint->x;
	long tempx = datapointx * 1000;
	tempx = tempx / SENSIVITY;
	datapoint->x = tempx;

	int16_t datapointy = datapoint->y;
	long tempy = datapointy * 1000;
	tempy = tempy / SENSIVITY;
	datapoint->y = tempy;

	int16_t datapointz = datapoint->z;
	long tempz = datapointz * 1000;
	tempz = tempz / SENSIVITY;
	datapoint->z = tempz;

}



void accelerometer_exe(){

	struct unit_t datapoint;
	readUnit(SLV_ADDR_ACC,ACCDATASTART_addr,&datapoint);
	accelerometer_dataProcess(&datapoint);
	//printUART_decimal(datapoint);
	printUART_hex(datapoint);
}





/*----------------------------------------
 * 				Gyroscope
 *----------------------------------------*/

void magnetic_exe(){

	struct unit_t datapoint;
	readUnit(SLV_ADDR_ACC,MAGDATASTART_addr,&datapoint);
	//accelerometer_dataProcess(&datapoint);

	double x = datapoint.x;
	double y = datapoint.y;

	double radAngle = atan2(y , x);
	double deg = radAngle * (180 / 3.14159265358979323846);
	int16_t degA = (int16_t)deg;

	char buffer[5];
	bool negative = flipbits(&degA);
	if(negative)
		UART_write_byte('-');
	myitoa(degA, buffer);

	UART_write_minimal(buffer,5);
	UART_write_byte(' ');

	//X
	char bufferX[5];
	negative = flipbits(&datapoint.x);
	myitoa(datapoint.x,bufferX);
	if(negative)
		UART_write_byte('-');
	UART_write_minimal(bufferX,5);
	UART_write_byte(' ');


	//Y
	char bufferY[5];
	negative = flipbits(&datapoint.y);
	myitoa(datapoint.y,bufferY);
	unsigned int delay = 0;
		while(delay<10000){delay++;}
	if(negative)
		UART_write_byte('-');
	UART_write_minimal(bufferY,5);




	UART_write_byte('\n');

	//printUART_decimal(datapoint);
	//printUART_hex(datapoint);
}


/*----------------------------------------
 * 				Public
 *---------------------------------------*/

void IMU_init(){
	accelerometer_init();
}

void IMU_readUnit(){
	accelerometer_exe();
	//magnetic_exe();
}











