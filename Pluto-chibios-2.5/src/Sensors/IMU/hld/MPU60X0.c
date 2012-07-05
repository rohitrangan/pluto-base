/*
 * MPU60X0.c
 *
 *  Created on: May 1, 2012
 *      Author: sapan
 */

#include "ch.h"
#include "hal.h"
#include "MPU60X0.h"
#include "chprintf.h"
#include "plutoInit.h"

/*
 * This function sets up the MPU and the I2C driver.
 * MPU registers can be read thereafter.
 * Added by rohitrangan 31-05-2012
 */
void set_mpu_i2c(void) {

	uint8_t txbuf[10], rxbuf[10] ;
	msg_t status = RDY_OK ;

	txbuf[0] = PWR_MGMT_1 ;
	txbuf[1] = 0x01 ;
	i2cAcquireBus(&I2C_MPU) ;
	status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 2, rxbuf, 0) ;
	i2cReleaseBus(&I2C_MPU) ;

	if(status != RDY_OK)
		return ;

	chThdSleepMilliseconds(35) ;

	txbuf[0] = GYRO_CONFIG ;
	txbuf[1] = 0x00 ;
	i2cAcquireBus(&I2C_MPU) ;
	status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 2, rxbuf, 0) ;
	i2cReleaseBus(&I2C_MPU) ;

	if(status != RDY_OK)
		return ;

	chThdSleepMilliseconds(35) ;

	txbuf[0] = ACCEL_CONFIG ;
	txbuf[1] = 0x00 ;
	i2cAcquireBus(&I2C_MPU) ;
	status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 2, rxbuf, 0) ;
	i2cReleaseBus(&I2C_MPU) ;

	if(status != RDY_OK)
		return ;

	chThdSleepMilliseconds(35) ;

	txbuf[0] = SMPRT_DIV ;
	txbuf[1] = 0x09 ;
	i2cAcquireBus(&I2C_MPU) ;
	status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 2, rxbuf, 0) ;
	i2cReleaseBus(&I2C_MPU) ;

	if(status != RDY_OK)
		return ;

	chThdSleepMilliseconds(35) ;

}
