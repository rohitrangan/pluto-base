/*Created By - Rohit Rangan
 *Created On - 23-06-2012
 *
 *This file contains the functions to get data from
 *the barometer.
 */

#include "ch.h"
#include "hal.h"
#include "BMP180.h"
#include "BarometerData.h"

/*If mode is 1, the function returns the temperature value
 *in 0.1 degrees Celsius. If mode is 2, the function returns
 *the pressure value in Pascals. No sleeps need to be given
 *when calling this function.
 */
int32_t readBarometerData(uint8_t mode) {
	uint8_t bmp_txbuf[2], bmp_rxbuf[3] ;
	int16_t reg1[8] ;
	uint16_t reg2[3] ;

	if(mode == BARO_TEMP_DATA) {
		uint32_t ut ;
		int32_t x1, x2, b5, tval ;
		bmp_txbuf[0] = BOSCH_CTL ;
		bmp_txbuf[1] = BOSCH_TEMP ;

		i2cAcquireBus(&I2C_BMP) ;
		i2cMasterTransmit(&I2C_BMP,  BMP_ADDR, bmp_txbuf, 2, bmp_rxbuf, 0) ;
		i2cReleaseBus(&I2C_BMP) ;
		chThdSleepMilliseconds(DELAY_TEMP) ;

	    bmp_txbuf[0] = BOSCH_ADC_MSB ;
	    i2cAcquireBus(&I2C_BMP) ;
	    i2cMasterTransmit(&I2C_BMP,  BMP_ADDR, bmp_txbuf, 1, bmp_rxbuf, 2) ;
	    i2cReleaseBus(&I2C_BMP) ;

	    ut = (bmp_rxbuf[0] << 8) + bmp_rxbuf[1] ;
	    read_bmp180_coefficient(reg1, reg2) ;
	    /*All calculations here are black magic from the
	     *BMP180 Datasheet. DO NOT CHANGE.
	     */
	    x1 = (ut - reg2[2]) * reg2[1] >> 15 ;
	    x2 = ((int32_t)reg1[6] << 11) / (x1 + reg1[7]) ;
	    b5 = x1 + x2 ;
	    tval = (b5 + 8) >> 4 ;
	    return tval ;
	}
	else if(mode == PRESSURE_DATA) {
		int32_t x1, x2, x3, b3, b5, b6, p, pval ;
		uint32_t b4, b7, up, ut ;

		/*To measure temperature as it is required for
		 *pressure measurement.
		 */
		bmp_txbuf[0] = BOSCH_CTL ;
		bmp_txbuf[1] = BOSCH_TEMP ;

		i2cAcquireBus(&I2C_BMP) ;
		i2cMasterTransmit(&I2C_BMP,  BMP_ADDR, bmp_txbuf, 2, bmp_rxbuf, 0) ;
		i2cReleaseBus(&I2C_BMP) ;
		chThdSleepMilliseconds(DELAY_TEMP) ;

		bmp_txbuf[0] = BOSCH_ADC_MSB ;
		i2cAcquireBus(&I2C_BMP) ;
		i2cMasterTransmit(&I2C_BMP,  BMP_ADDR, bmp_txbuf, 1, bmp_rxbuf, 2) ;
		i2cReleaseBus(&I2C_BMP) ;

		ut = (bmp_rxbuf[0] << 8) + bmp_rxbuf[1] ;

		/*Pressure measurement starts now */
		bmp_txbuf[0] = BOSCH_CTL ;
		bmp_txbuf[1] = MEASUREMENT_MODE ;

		i2cAcquireBus(&I2C_BMP) ;
		i2cMasterTransmit(&I2C_BMP,  BMP_ADDR, bmp_txbuf, 2, bmp_rxbuf, 0) ;
		i2cReleaseBus(&I2C_BMP) ;

		/* Wait until pressure measurement is done. Time required for this depends on selected mode. */
		chThdSleepMilliseconds(DELAY_PRESSURE) ;

	    bmp_txbuf[0] = BOSCH_ADC_MSB ;

	    i2cAcquireBus(&I2C_BMP) ;
	    i2cMasterTransmit(&I2C_BMP, BMP_ADDR, bmp_txbuf, 1, bmp_rxbuf, 3) ;
	    i2cReleaseBus(&I2C_BMP) ;

	    /*All calculations here are black magic from the
	     *BMP180 Datasheet. DO NOT CHANGE.
	     */
	    up = ((bmp_rxbuf[0] << 16) + (bmp_rxbuf[1] << 8) + bmp_rxbuf[2]) >> (8 - OSS) ;
	    read_bmp180_coefficient(reg1, reg2) ;

	    x1 = (ut - reg2[2]) * reg2[1] >> 15 ;
	    x2 = ((int32_t)reg1[6] << 11) / (x1 + reg1[7]) ;
	    b5 = x1 + x2 ;

	    b6 = b5 - 4000 ;
	    x1 = (reg1[4] * (b6 * b6 >> 12)) >> 11 ;
	    x2 = reg1[1] * b6 >> 11 ;
	    x3 = x1 + x2 ;
	    b3 = ((((int32_t)reg1[0] * 4 + x3) << OSS) + 2) >> 2 ;

	    x1 = reg1[2] * b6 >> 13 ;
	    x2 = (reg1[3] * (b6 * b6 >> 12)) >> 16 ;
	    x3 = ((x1 + x2) + 2) >> 2 ;
	    b4 = (reg2[0] * (uint32_t)(x3 + 32768)) >> 15 ;
	    b7 = ((uint32_t)up - b3) * (50000 >> OSS) ;

	    if(b7 < 0x80000000)
	    	p = (b7 * 2) / b4 ;
	    else
	    	p = (b7 / b4) * 2 ;

	    x1 = (p >> 8) * (p >> 8) ;
	    x1 = (x1 * 3038) >> 16 ;
	    x2 = (-7357 * p) >> 16 ;
	    pval = p + ((x1 + x2 + 3791) >> 4) ;
	    return pval ;
	}
	else {
		return 0 ;
	}
}

