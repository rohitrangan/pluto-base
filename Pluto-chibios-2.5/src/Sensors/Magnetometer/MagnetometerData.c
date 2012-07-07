/*Created By - Rohit Rangan
 *Created On - 22-06-2012
 *
 *This file contains the functions to get data from
 *the magnetometer.
 */
#include<math.h>

#include "ch.h"
#include "hal.h"
#include "utils.h"
#include "hmc5883.h"
#include "IMUData.h"
#include "MagnetometerData.h"

/*The raw values are stored in val. To convert to microTesla,
 *multiply val[i] by MAG_RANGE.
 */
void readMagnetometerData(int16_t val[3]) {
	uint8_t hmc_txbuf[2], hmc_rxbuf[6] ;
	/*Send Single Measurement query. Write 0x01 to MODE Register */
	hmc_txbuf[0] = MODE ;
	hmc_txbuf[1] = 0x01 ;

	i2cAcquireBus(&I2C_HMC) ;
	i2cMasterTransmit(&I2C_HMC, HMC_ADDR, hmc_txbuf, 2, hmc_rxbuf, 0) ;
	i2cReleaseBus(&I2C_HMC) ;

	chThdSleepMicroseconds(30) ; /* Sleep for appropriate time - According to ODR */
	/*Alternatively Check status of HMC_DRDY_PIN to check availability of new data */
	/*while(palReadPad(HMC_DRDY_PORT, HMC_DRDY_PIN)) {
		chprintf((BaseSequentialStream *)&SD1, "N") ;
	}*/

	hmc_txbuf[0] = OUT_X_MSB ;
	i2cAcquireBus(&I2C_HMC) ;
	i2cMasterTransmit(&I2C_HMC, HMC_ADDR, hmc_txbuf, 1, hmc_rxbuf, 6) ;
	i2cReleaseBus(&I2C_HMC) ;

	val[0] = (hmc_rxbuf[0] << 8) + hmc_rxbuf[1] ;
	val[2] = (hmc_rxbuf[2] << 8) + hmc_rxbuf[3] ;
	val[1] = (hmc_rxbuf[4] << 8) + hmc_rxbuf[5] ;
}

/*This function calculates the magnetometer data in uT and
 *is stored in val.
 */
void magGetScaledData(float val[3]) {
	int16_t mag[3] ;
	readMagnetometerData(mag) ;

	val[0] = (float)mag[0] * MAG_RANGE ;
	val[1] = (float)mag[1] * MAG_RANGE ;
	val[2] = (float)mag[2] * MAG_RANGE ;
}

/*Returns the heading of the board in degrees.
 *000 -> North.
 *090 -> East.
 *180 -> South.
 *270 -> West.
 */
float getHeading(void) {
	float mag[3] ;
	float heading, angles[3], cosRoll, sinRoll, cosPitch, sinPitch ;

	eulerAngles(angles) ;
	angles[0] = (angles[0] * M_PI / 180) ;
	angles[1] = (angles[1] * M_PI / 180) ;
	cosPitch = cosf(angles[0]) ;
	sinPitch = sinf(angles[0]) ;
	cosRoll  = cosf(angles[1]) ;
	sinRoll  = sinf(angles[1]) ;

	magGetScaledData(mag) ;

	/*float Xh = (mag[0] * cosPitch) + (mag[1] * sinPitch * sinRoll) - (mag[2] * sinPitch * cosRoll) ;
	float Yh = (mag[1] * cosRoll) + (mag[2] * sinRoll) ;*/
	float Xh = (mag[0] * cosPitch) + (mag[2] * sinPitch) ;
	float Yh = (mag[0] * sinRoll * sinPitch) + (mag[1] * cosRoll) - (mag[2] * sinRoll * cosPitch) ;
	heading = my_atan2f(Yh, Xh) ;

	return (heading * 180.0 / M_PI) ;
}
