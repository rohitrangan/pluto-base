/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains the functions to get data from
 *the accelerometer.
 */
#include<math.h>

#include "pluto.h"

#if PLUTO_USE_ZIGBEE
/* Serial Driver for Output. */
#define OUTPUT					SD5
#else
#define OUTPUT					SD1
#endif /* PLUTO_USE_ZIGBEE*/

#if PLUTO_USE_IMU
/*If Mode is ACCEL_DATA then the accelerometer values
 *are read, mode is GYRO_DATA then gyrometer values
 *are read, if mode is TEMP_DATA then the temperature
 *value is read. val stores the values of the sensor
 *data.
 */
void readIMUData(uint8_t mode, IMUData *imudata) {
	uint8_t txbuf[1], rxbuf[6], i ;
	msg_t status = RDY_OK ;

	for(i = 0 ; i < 6 ; i++)
		rxbuf[i] = 0x00 ;

	switch(mode) {
		case ACCEL_DATA 	: txbuf[0] = ACCEL_XOUT_H ;
				  	  	  	  i2cAcquireBus(&I2C_MPU) ;
				  	  	  	  status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 1, rxbuf, 6) ;
				  	  	  	  i2cReleaseBus(&I2C_MPU) ;

				  	  	  	  if(status != RDY_OK)
				  	  	  		  return ;

				  	  	  	  imudata->RAW_ACCEL_X = (rxbuf[0] << 8) + rxbuf[1] ;
				  	  	  	  imudata->RAW_ACCEL_Y = (rxbuf[2] << 8) + rxbuf[3] ;
				  	  	  	  imudata->RAW_ACCEL_Z = (rxbuf[4] << 8) + rxbuf[5] ;

				  	  	  	  imudata->ACCEL_X = - (float)imudata->RAW_ACCEL_X / imudata->ACCEL_SENS ; // rrangan as the direction on the device is on the reverse side.
				  	  	      imudata->ACCEL_Y = (float)imudata->RAW_ACCEL_Y / imudata->ACCEL_SENS ;
				  	  	      imudata->ACCEL_Z = - (float)imudata->RAW_ACCEL_Z / imudata->ACCEL_SENS ; // rrangan as the direction on the device is on the reverse side.
				  	  	  	  break ;

		case GYRO_DATA  	: txbuf[0] = GYRO_XOUT_H ;
				  	  	  	  i2cAcquireBus(&I2C_MPU) ;
				  	  	  	  status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 1, rxbuf, 6) ;
				  	  	  	  i2cReleaseBus(&I2C_MPU) ;

				  	  	  	  if(status != RDY_OK)
				  	  	  		  return ;

				  	  	  	  imudata->RAW_GYRO_X = (rxbuf[0] << 8) + rxbuf[1] ;
				  	  	      imudata->RAW_GYRO_Y = (rxbuf[2] << 8) + rxbuf[3] ;
				  	  	  	  imudata->RAW_GYRO_Z = (rxbuf[4] << 8) + rxbuf[5] ;

				  	  	  	  imudata->GYRO_X = - ((float)imudata->RAW_GYRO_X / imudata->GYRO_SENS) * (M_PI / 180.0) ; // rrangan as the direction on the device is on the reverse side.
				  	  	      imudata->GYRO_Y = ((float)imudata->RAW_GYRO_Y / imudata->GYRO_SENS) * (M_PI / 180.0) ;
				  	  	      imudata->GYRO_Z = - ((float)imudata->RAW_GYRO_Z / imudata->GYRO_SENS) * (M_PI / 180.0) ; // rrangan as the direction on the device is on the reverse side.
				  	  	  	  break ;

		case IMU_TEMP_DATA  : txbuf[0] = TEMP_OUT_H ;
		  	  	  	  	  	  i2cAcquireBus(&I2C_MPU) ;
		  	  	  	  	  	  status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 1, rxbuf, 2) ;
		  	  	  	  	  	  i2cReleaseBus(&I2C_MPU) ;

		  	  	  	  	  	  if(status != RDY_OK)
		  	  	  	  	  		  return ;

		  	  	  	  	  	  imudata->RAW_TEMP = (rxbuf[0] << 8) + rxbuf[1] ;
		  	  	  	  	  	  break ;

		default 	   	   : return ;
	}
}

/*To read all the accelerometer and gyrometer values
 *at the same time for calculating attitude.
 *USE THIS FUNCTION ONLY WHEN ATTITUDE CALCULATION IS
 *REQUIRED.
 */
void readAllIMUData(IMUData *imudata) {
	uint8_t txbuf[1], rxbuf[14] ;
	msg_t status = RDY_OK ;

	txbuf[0] = ACCEL_XOUT_H ;
	i2cAcquireBus(&I2C_MPU) ;
	status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 1, rxbuf, 14) ;
	i2cReleaseBus(&I2C_MPU) ;

	if(status != RDY_OK)
		return ;

	imudata->RAW_ACCEL_X = (rxbuf[0] << 8 ) + rxbuf[1]  ;
	imudata->RAW_ACCEL_Y = (rxbuf[2] << 8 ) + rxbuf[3]  ;
	imudata->RAW_ACCEL_Z = (rxbuf[4] << 8 ) + rxbuf[5]  ;
	imudata->RAW_GYRO_X  = (rxbuf[8] << 8 ) + rxbuf[9]  ;
	imudata->RAW_GYRO_Y  = (rxbuf[10] << 8) + rxbuf[11] ;
	imudata->RAW_GYRO_Z  = (rxbuf[12] << 8) + rxbuf[13] ;

	imudata->ACCEL_X = - (float)imudata->RAW_ACCEL_X / imudata->ACCEL_SENS ; // rrangan as the direction on the device is on the reverse side.
	imudata->ACCEL_Y = (float)imudata->RAW_ACCEL_Y / imudata->ACCEL_SENS ;
	imudata->ACCEL_Z = - (float)imudata->RAW_ACCEL_Z / imudata->ACCEL_SENS ; // rrangan as the direction on the device is on the reverse side.
	imudata->GYRO_X  = - ((float)imudata->RAW_GYRO_X / imudata->GYRO_SENS) * (M_PI / 180.0) ; // rrangan as the direction on the device is on the reverse side.
	imudata->GYRO_Y  = ((float)imudata->RAW_GYRO_Y / imudata->GYRO_SENS) * (M_PI / 180.0) ;
	imudata->GYRO_Z  = - ((float)imudata->RAW_GYRO_Z / imudata->GYRO_SENS) * (M_PI / 180.0) ; // rrangan as the direction on the device is on the reverse side.
}

/*This function calculates the pitch, roll and
 *yaw. It calls readAccelerometerData inside.
 *The angles are stored in the order pitch,
 *roll and yaw.
 *
 *TODO rohitrangan Merge Gyro Data.
 */
void eulerAngles(IMUData *imudata, float angles[3]) {
	readIMUData(ACCEL_DATA, imudata) ;

	angles[0] = asinf((-1.0f *imudata->ACCEL_X)) ;
	angles[0] *= 180.0 ;
	angles[0] /= M_PI ;

	angles[1] = atanf((imudata->ACCEL_Y / imudata->ACCEL_Z)) ;
	angles[1] *= 180.0 ;
	angles[1] /= M_PI ;

	angles[2] = 0.0 ;
}

void displayCoeffs(void) {
	chprintf((BaseSequentialStream *)&OUTPUT, "Gyro X a:- %f ", GYRO_X_A) ;
	chprintf((BaseSequentialStream *)&OUTPUT, "Gyro X b:- %f ", GYRO_X_B) ;
	chprintf((BaseSequentialStream *)&OUTPUT, "Gyro Y a:- %f ", GYRO_Y_A) ;
	chprintf((BaseSequentialStream *)&OUTPUT, "Gyro Y b:- %f ", GYRO_Y_B) ;
	chprintf((BaseSequentialStream *)&OUTPUT, "Gyro Z a:- %f ", GYRO_Z_A) ;
	chprintf((BaseSequentialStream *)&OUTPUT, "Gyro Z b:- %f\r\n", GYRO_Z_B) ;
}
#endif	/*PLUTO_USE_IMU */
