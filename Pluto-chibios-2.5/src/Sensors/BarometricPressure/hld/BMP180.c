/*
 * BMP180.c
 *
 *  Created on: Apr 28, 2012
 *      Author: sapan
 */

#include "ch.h"
#include "hal.h"
#include "BMP180.h"
#include "chprintf.h"

uint8_t measurement_mode = 0x00, bmp_txbuf[1],bmp_rxbuf[22], oss = 0;
int16_t delay_bmp180 = 0, delay_bmp180_temperature = 0;

/* This function takes operating_mode as input and sets value of measurement_mode and
 * delay required between sending measurement command and reading data register.
 * 0 = ultra_low power mode.		delay = 6 ms (4.5 ms as per datasheet)
 * 1 = standard mode.				delay = 9 ms (7.5 ms as per datasheet)
 * 2 = high resoltuion mode.		delay = 15 ms (13.5 ms as per datasheet)
 * 3 = ultra high resolution mode.	delay = 27 ms (25.5 ms as per datasheet)
 *
 */

void initialize_bmp180(uint8_t operating_mode) {
	delay_bmp180_temperature = 5; //4.5 ms as per datasheet
	oss = operating_mode;
	if(operating_mode == 0) {
		measurement_mode = BOSCH_OSS0;
		delay_bmp180 = 6;
	}
	if(operating_mode == 1) {
		measurement_mode = BOSCH_OSS1;
		delay_bmp180 = 9;
	}
	if(operating_mode == 2) {
		measurement_mode = BOSCH_OSS2;
		delay_bmp180 = 15;
	}
	if(operating_mode == 3) {
		measurement_mode = BOSCH_OSS3;
		delay_bmp180 = 27;
	}
}

void read_bmp180_coefficient(int16_t reg1[8], uint16_t reg2[3]) {
  /* wait until sensor inits */
	chThdSleepMilliseconds(20); //(datasheet 10 ms)

	bmp_txbuf[0] = 0xAA;
	i2cAcquireBus(&I2C_BMP) ;
	i2cMasterTransmit(&I2C_BMP, BMP_ADDR, bmp_txbuf, 1, bmp_rxbuf, 22);
	i2cReleaseBus(&I2C_BMP) ;

	reg1[0] = (bmp_rxbuf[0] << 8) + bmp_rxbuf[1];	/*AC1 */
	reg1[1] = (bmp_rxbuf[2] << 8) + bmp_rxbuf[3];	/*AC2 */
	reg1[2] = (bmp_rxbuf[4] << 8) + bmp_rxbuf[5];	/*AC3 */
	reg2[0] = (bmp_rxbuf[6] << 8) + bmp_rxbuf[7];	/*AC4 */
	reg2[1] = (bmp_rxbuf[8] << 8) + bmp_rxbuf[9];	/*AC5 */
	reg2[2] = (bmp_rxbuf[10] << 8) + bmp_rxbuf[11];	/*AC6 */
	reg1[3] = (bmp_rxbuf[12] << 8) + bmp_rxbuf[13];	/*B1 */
	reg1[4] = (bmp_rxbuf[14] << 8) + bmp_rxbuf[15];	/*B2 */
	reg1[5] = (bmp_rxbuf[16] << 8) + bmp_rxbuf[17];	/*MB */
	reg1[6] = (bmp_rxbuf[18] << 8) + bmp_rxbuf[19];	/*MC */
	reg1[7] = (bmp_rxbuf[20] << 8) + bmp_rxbuf[21];	/*MD */

}

