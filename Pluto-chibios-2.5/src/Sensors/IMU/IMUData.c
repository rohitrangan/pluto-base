/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains the functions to get data from
 *the accelerometer.
 */
#include<math.h>

#include "MPU60X0.h"

/*If Mode is 1 then the accelerometer values
 *are read, mode is 2 then gyrometer values
 *are read, if mode is 3 then the temperature
 *value is read. val stores the values of the
 *sensor data.
 *TODO - rohitrangan output the data in engineering format.
 */
void readIMUData(uint8_t mode, int16_t val[3]) {

	uint8_t txbuf[6], rxbuf[6], i ;
	msg_t status = RDY_OK ;

	for(i = 0 ; i < 6 ; i++)
		rxbuf[i] = 0x00 ;

	switch(mode) {
		case 1  : txbuf[0] = ACCEL_XOUT_H ;
				  i2cAcquireBus(&I2C_MPU) ;
				  status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 1, rxbuf, 6) ;
				  i2cReleaseBus(&I2C_MPU) ;

				  if(status != RDY_OK)
					  return ;

				  val[0] = (rxbuf[0] << 8) + rxbuf[1] ;
				  val[1] = (rxbuf[2] << 8) + rxbuf[3] ;
				  val[2] = (rxbuf[4] << 8) + rxbuf[5] ;
				  break ;

		case 2  : txbuf[0] = GYRO_XOUT_H ;
				  i2cAcquireBus(&I2C_MPU) ;
				  status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 1, rxbuf, 6) ;
				  i2cReleaseBus(&I2C_MPU) ;

				  if(status != RDY_OK)
					  return ;

		  	  	  val[0] = (rxbuf[0] << 8) + rxbuf[1] ;
		  	  	  val[1] = (rxbuf[2] << 8) + rxbuf[3] ;
		  	  	  val[2] = (rxbuf[4] << 8) + rxbuf[5] ;
		  	  	  break ;

		case 3  : txbuf[0] = TEMP_OUT_H ;
		  	  	  i2cAcquireBus(&I2C_MPU) ;
		  	  	  status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 1, rxbuf, 2) ;
		  	  	  i2cReleaseBus(&I2C_MPU) ;

		  	  	  if(status != RDY_OK)
		  	  		  return ;

				  val[0] = (rxbuf[0] << 8) + rxbuf[1] ;
				  break ;

		default : return ;
	}

}

/*This function calculates the pitch, roll and
 *yaw. It calls readAccelerometerData inside.
 *The angles are stored in the order pitch,
 *roll and yaw.
 */
#if CORTEX_USE_FPU
void eulerAngles(float angles[3]) {

	int16_t acc[3] ;
	readIMUData(1, acc) ;
	chThdSleepMilliseconds(10) ;

	angles[1] = atanf(((float)acc[1] / (float)acc[2])) ;
	angles[1] *= 180.0 ;
	angles[1] /= M_PI ;

	float g = sqrt((pow((float)acc[0], 2) + pow((float)acc[1], 2) + pow((float)acc[2], 2))) ;

	angles[0] = asinf((-1.0 * (float)acc[0] / g)) ;
	angles[0] *= 180.0 ;
	angles[0] /= M_PI ;

	angles[2] = 0.0 ;
}
#endif	/*CORTEX_USE_FPU */
