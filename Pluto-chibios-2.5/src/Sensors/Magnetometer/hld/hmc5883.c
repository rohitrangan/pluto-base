/*
 * hmc5883.c
 *
 *  Created on: Apr 26, 2012
 *      Author: sapan
 */
#include "ch.h"
#include "hal.h"
#include "hmc5883.h"
#include "chprintf.h"

uint8_t hmc_txbuf[20], hmc_rxbuf[20] ;
float hmc_range = 0.0 ;

/*
 * average = No. of samples to be average per measurement
 * ODR = Output Data Rate
 * Mode = Normal, Positive Bias or Negative Bias...Refer to Data Sheet for further details
 * Gain = Range of Magnetic Field
 * OP_Mode = Continuous, Single measurement or Idle mode
 */

void initialize_HMC(uint8_t average, uint8_t ODR, uint8_t Mode, uint8_t Gain, uint8_t OP_Mode) {
	hmc_txbuf[1] = 0x00;
	/*Put Address of CONFIG_A in hmc_txbuf[0] */
	hmc_txbuf[0] = CONFIG_A;
	/*Set value for CONFIG_A register */
	hmc_txbuf[1] |= average;
	hmc_txbuf[1] = hmc_txbuf[1] << 5;
	hmc_txbuf[1] |= (ODR << 2);
	hmc_txbuf[1] |= Mode;
	i2cAcquireBus(&I2C_HMC) ;
	i2cMasterTransmit(&I2C_HMC, HMC_ADDR, hmc_txbuf, 2, hmc_rxbuf, 0);
	i2cReleaseBus(&I2C_HMC) ;

	hmc_txbuf[1] = 0x00;
	/*Put Address of CONFIG_B in hmc_txbuf[0] */
	hmc_txbuf[0] = CONFIG_B;
	/*Set value for CONFIG_B register */
	hmc_txbuf[1] |= (Gain << 5);
	i2cAcquireBus(&I2C_HMC) ;
	i2cMasterTransmit(&I2C_HMC, HMC_ADDR, hmc_txbuf, 2, hmc_rxbuf, 0);
	i2cReleaseBus(&I2C_HMC) ;

	hmc_txbuf[1] = 0x00;
	/*Put Address of MODE register in hmc_txbuf[0] */
	hmc_txbuf[0] = MODE;
	/*Set value for MODE register */
	hmc_txbuf[1] = OP_Mode;
	i2cAcquireBus(&I2C_HMC) ;
	i2cMasterTransmit(&I2C_HMC, HMC_ADDR, hmc_txbuf, 2, hmc_rxbuf, 0);
	i2cReleaseBus(&I2C_HMC) ;

	if(Gain == 0)
		hmc_range = 0.73;
	if(Gain == 1)
		hmc_range = 0.92;
	if(Gain == 2)
		hmc_range = 1.22;
	if(Gain == 3)
		hmc_range = 1.52;
	if(Gain == 4)
		hmc_range = 2.27;
	if(Gain == 5)
		hmc_range = 2.56;
	if(Gain == 6)
		hmc_range = 3.03;
	if(Gain == 7)
		hmc_range = 4.35;

}


void read_hmc_data(void) {
	int16_t hmc_rawx =0, hmc_rawy = 0, hmc_rawz =0;
	int32_t  x_mag = 0.0, y_mag = 0.0, z_mag =0.0;
	/*Send Single Measurement query. Write 0x01 to MODE Register */
	hmc_txbuf[0] = MODE;
	hmc_txbuf[1] = 0x01;

	/*Acquire I2C bus before initiating any transfer */

	i2cAcquireBus(&I2C_HMC) ;
	i2cMasterTransmit(&I2C_HMC, HMC_ADDR, hmc_txbuf, 2, hmc_rxbuf, 0);
	i2cReleaseBus(&I2C_HMC) ;

	/*Relese I2C bus after transfer is done */

	chThdSleepMilliseconds(30); /* Sleep for appropriate time - According to ODR */
	/*Alternatively Check status of HMC_DRDY_PIN to check availability of new data */
	/*while(palReadPad(HMC_DRDY_PORT, HMC_DRDY_PIN)) {
		chprintf((BaseSequentialStream *)&SD1, "N");
	}*/

	i2cAcquireBus(&I2C_HMC) ;
	i2cMasterReceive(&I2C_HMC, HMC_ADDR, hmc_rxbuf, 6);
	i2cReleaseBus(&I2C_HMC) ;

	/*These are just test points. Remove these once proper working of these function is confirmed
	 * and instead of these use pointer to pass values to user space.
	 */
	hmc_rawx = (hmc_rxbuf[0] << 8) + hmc_rxbuf[1];
	hmc_rawy = (hmc_rxbuf[2] << 8) + hmc_rxbuf[3];
	hmc_rawz = (hmc_rxbuf[4] << 8) + hmc_rxbuf[5];

	/*Convert raw value in nano Tesla */
	x_mag = (hmc_range * hmc_rawx) / 10 ;
	y_mag = (hmc_range * hmc_rawy) / 10 ;
	z_mag = (hmc_range * hmc_rawz) / 10 ;

	//x_mag = (int32_t)(x_mag*exp_float);

	chprintf((BaseSequentialStream *)&SD1, "Raw and Field: \t%d,\t%d,\t%d,\t%D,\t%D,\t%D \t%D\r\n", hmc_rawx, hmc_rawy, hmc_rawz, x_mag, y_mag, z_mag, hmc_range) ;
}
/*Not behaving as per expectations. Resolve issue of reading value of HMC register */

void read_hmc_register(void){

	uint8_t x = 0;

	i2cAcquireBus(&I2C_HMC) ;
	i2cMasterReceive(&I2C_HMC, HMC_ADDR, hmc_rxbuf,20);
	i2cReleaseBus(&I2C_HMC) ;
	for(x = 0 ; x < 20 ; x++) {
		chprintf((BaseSequentialStream *)&SD1,"Register Address:%x, Value: %x\r\n",x,hmc_rxbuf[x] );
	}


}



