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

#define SENSIVITY 16675 //this is (faceup - facedown)/2 - not converted to v/g

void IMU_dataProcess(struct unit_t datapoint, unsigned char data[]);

//Initialize Accelerometer
void IMU_init(){

		I2C_writeSlv(SLV_ADDR_GYRO,CTRL1_addr,0x0F);

		//Data update at 50Hz, enable X, Y, Z
		I2C_writeSlv(SLV_ADDR_ACC,CTRL0_addr,CTRL0);
		I2C_writeSlv(SLV_ADDR_ACC,CTRL1_addr,CTRL1);
		I2C_writeSlv(SLV_ADDR_ACC,CTRL2_addr,CTRL2);
		I2C_writeSlv(SLV_ADDR_ACC,CTRL3_addr,CTRL3);
		I2C_writeSlv(SLV_ADDR_ACC,CTRL4_addr,CTRL4);
		I2C_writeSlv(SLV_ADDR_ACC,CTRL5_addr,CTRL5);
		I2C_writeSlv(SLV_ADDR_ACC,CTRL6_addr,CTRL6);
		I2C_writeSlv(SLV_ADDR_ACC,CTRL7_addr,CTRL7);

		char *data = (char*)malloc(8);
		I2C_readSlvChunk(SLV_ADDR_ACC,CTRL0_addr,8,data);
		//UART_write(data,8);

}

void IMU_readUnit(){
	struct unit_t datapoint;
	int16_t temp;
	int16_t temp2;
	int i = 0;
	int j = 0;
	char data[6];// = (char*)malloc(6);
	I2C_readSlvChunk(SLV_ADDR_ACC,DATASTART_addr,6,data);

	/*
	//TEST CODE
	data[0] = 0xAA;
	data[1] = 0x80;
	data[2] = 0xAA;
	data[3] = 0x80;
	data[4] = 0xAA;
	data[5] = 0x80;
	//UART_write(data,6);
	*/

	for(i=0; i<6; i++){
		//temp = 0;
		if(i%2==0){
			temp = data[i];
		}
		else{
			temp |= 0xFF00;
			temp2 = (data[i]<<8) | 0x00FF;
			temp &= temp2;
			if(j==0) 					//x
				datapoint.x = temp;
			else if(j==1) 				//y
				datapoint.y = temp;
			else						//z
				datapoint.z = temp;
			j++;
			temp = 0;
		}
	}

	//UART_writeln(data,6);

	//IMU_printUART(datapoint);

	unsigned char processed_array[6];
	IMU_dataProcess(datapoint,processed_array);
	UART_write((const char*)processed_array,6);
	UART_write_byte(0x5C);
	UART_write_byte(0x6E);
}

void IMU_dataProcess(struct unit_t datapoint, unsigned char data[]){

	int16_t datapointx = datapoint.x;
	long tempx = datapointx * 1000;
	tempx = tempx / SENSIVITY;
	uint16_t calx = tempx;

	int16_t datapointy = datapoint.y;
	long tempy = datapointy * 1000;
	tempy = tempy / SENSIVITY;
	uint16_t caly = tempy;

	int16_t datapointz = datapoint.z;
	long tempz = datapointz * 1000;
	tempz = tempz / SENSIVITY;
	uint16_t calz = tempz;


	data[0] = (calx >> 8);
	data[1] = (calx & 0x0000FFFF);

	data[2] = (caly >> 8);
	data[3] = (caly & 0x0000FFFF);

	data[4] = (calz >> 8);
	data[5] = (calz & 0x0000FFFF);

}


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

void IMU_printUART(struct unit_t datapoint){
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


	/*
	int i=4;
	for(; testdata && i ; --i, testdata /= 10)
			bufferx[i] = "0123456789abcdef"[testdata % 10];
	for(;i>=0;i--)
			bufferx[i] = '0';
	*/

	//bufferx = myitoa(testdata);

	//myitoa(testdata,bufferx);

	//myitoa(datapoint.y,buffery);
	//myitoa(datapoint.z,bufferz);
	//sprintf(bufferx,"%d",12);
	//sprintf(buffery,"%d",datapoint.y);
	//sprintf(bufferz,"%d",datapoint.z);

	//UART_writeln(buffery,20);
	//UART_writeln(bufferz,20);
}
