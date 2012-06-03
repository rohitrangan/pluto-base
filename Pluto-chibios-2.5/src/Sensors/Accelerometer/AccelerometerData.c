/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains the functions to get data from
 *the accelerometer.
 */

#include "MPU60X0.h"

/*If Mode is 1 then the accelerometer values
 *are read, mode is 2 then gyrometer values
 *are read, if mode is 3 then the temperature
 *value is read. val stores the values of the
 *sensor data.
 *TODO - rohitrangan output the data in engineering format.
 */
void readAccelerometerData(uint8_t mode, int16_t val[3]) {

	uint8_t txbuf[6], rxbuf[6], i ;

	for(i = 0 ; i < 6 ; i++)
		rxbuf[i] = 0x00 ;

	switch(mode) {
		case 1  : txbuf[0] = ACCEL_XOUT_H ;
				  i2cMasterTransmit(&I2CD1, MPU_ADDR, txbuf, 1, rxbuf, 6) ;
				  val[0] = (rxbuf[0] << 8) + rxbuf[1] ;
				  val[1] = (rxbuf[2] << 8) + rxbuf[3] ;
				  val[2] = (rxbuf[4] << 8) + rxbuf[5] ;
				  break ;
		case 2  : txbuf[0] = GYRO_XOUT_H ;
		  	  	  i2cMasterTransmit(&I2CD1, MPU_ADDR, txbuf, 1, rxbuf, 6) ;
		  	  	  val[0] = (rxbuf[0] << 8) + rxbuf[1] ;
		  	  	  val[1] = (rxbuf[2] << 8) + rxbuf[3] ;
		  	  	  val[2] = (rxbuf[4] << 8) + rxbuf[5] ;
		  	  	  break ;
		case 3  : txbuf[0] = TEMP_OUT_H ;
				  i2cMasterTransmit(&I2CD1, MPU_ADDR, txbuf, 1, rxbuf, 2) ;
				  val[0] = (rxbuf[0] << 8) + rxbuf[1] ;
				  break ;
		default : return ;
	}

}
