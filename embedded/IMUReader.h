/*
 * IMUReader.h
 *
 *  Created on: Oct 3, 2015
 *      Author: dulimon
 */
#include <stdint.h>
#ifndef IMUREADER_H_
#define IMUREADER_H_

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

#define ACC_CTRL0  0x000   //for normal mode, filters by-passed
#define ACC_CTRL1  0x4F   //update after read, and all axes of acceleration enabled at 25Hz
#define ACC_CTRL2  0xC1   //50Hz anti-alias, +/- 2g, no self-test, (SPI 3-wire)
#define ACC_CTRL3  0x00   //No INT1 actions
#define ACC_CTRL4  0x08   //accelerometer data ready on INT2.
#define ACC_CTRL5  0x0E   //No temperature, low-res magnetic, 25Hz, latch interrupt on INT2
#define ACC_CTRL6  0x00   //+/-2gauss sensitivity.
#define ACC_CTRL7  0x00   //no normal high-pass acceleration filter, no Temp, magnetic always on, continuous



#define ACCDATASTART_addr 0x28
#define MAGDATASTART_addr 0x08
#define MAGDATAOFFSETSTART_addr 0x16

struct unit_t{
	int16_t x;
	int16_t y;
	int16_t z;
};

void IMU_init();
void IMU_readUnit();
void IMU_printUART(struct unit_t data);
#endif /* IMUREADER_H_ */
