/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains the functions to get data from
 *the accelerometer.
 */

#ifndef IMUDATA_H_
#define IMUDATA_H_

#include "ch.h"

/*To get data from the different sensors on the IMU.
 */
#define ACCEL_DATA		1
#define GYRO_DATA		2
#define IMU_TEMP_DATA	3

/*The sensitivities of the Acclerometer and the Gyro.
 */
#define GYRO_XSENS	131.0f
#define GYRO_YSENS 	131.0f
#define GYRO_ZSENS 	131.0f

#define ACCEL_XSENS 8192
#define ACCEL_YSENS 8192
#define ACCEL_ZSENS 8192

/*If Mode is ACCEL_DATA then the accelerometer values
 *are read, mode is GYRO_DATA then gyrometer values
 *are read, if mode is TEMP_DATA then the temperature
 *value is read. val stores the values of the sensor
 *data.
 */
void readIMUData(uint8_t mode, int16_t val[3]) ;

/*This function calculates the pitch, roll and yaw. It
 *calls readAccelerometerData inside. The angles are
 *stored in the order pitch, roll and yaw.
 */
#if CORTEX_USE_FPU
void eulerAngles(float angles[3]) ;
#endif	/*CORTEX_USE_FPU */

#endif	/*IMUDATA_H_ */
