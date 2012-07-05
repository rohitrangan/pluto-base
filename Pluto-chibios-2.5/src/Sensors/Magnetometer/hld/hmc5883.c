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
