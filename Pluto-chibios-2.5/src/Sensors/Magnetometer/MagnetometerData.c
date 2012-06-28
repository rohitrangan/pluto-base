/*Created By - Rohit Rangan
 *Created On - 22-06-2012
 *
 *This file contains the functions to get data from
 *the magnetometer.
 */
#include<math.h>

#include "ch.h"
#include "hal.h"
#include "hmc5883.h"

/*The raw values are stored in val. To convert to nano Tesla,
 *multiply val[i] by HMC_RANGE_CONVERT and divide by 10.
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
	val[1] = (hmc_rxbuf[2] << 8) + hmc_rxbuf[3] ;
	val[2] = (hmc_rxbuf[4] << 8) + hmc_rxbuf[5] ;
}

#if CORTEX_USE_FPU
/*Returns the heading of the board in degrees.
 *000 -> North.
 *090 -> East.
 *180 -> South.
 *270 -> West.
 */
float getHeading(void) {
	int16_t data[3] ;
	float heading = 0.0 ;

	readMagnetometerData(data) ;

	heading = atan2f((float)data[1], (float)data[0]) ;
	heading += 0.01309 ;

	if(heading < 0)
		heading += (2 * M_PI) ;

	if(heading > (2 * M_PI))
		heading -= (2 * M_PI) ;

	return (heading * 180.0 / M_PI) ;
}
#endif	/*CORTEX_USE_FPU */
