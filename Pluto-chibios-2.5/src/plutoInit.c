/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains all the functions required to start
 *different drivers like I2C, SPI and USART.
 */

#include "plutoconf.h"
#include "ch.h"
#include "hal.h"
#include "ff.h"

#include "plutoInit.h"
#include "hmc5883.h"
#include "BMP180.h"
#include "MPU60X0.h"
#include "ms4515.h"

/*Config for USART1 */
static const SerialConfig usart1 = {
    115200,
    0,
    USART_CR2_STOP1_BITS | USART_CR2_LINEN,
    0
};

#if PLUTO_USE_FATFS
/*High speed SPI configuration */
const SPIConfig hs_spicfg = {
	NULL, /* No callback */
	/* Hardware definition starts */
	GPIOA, /* Port on which slave select line is defined */
	GPIOA_CS_SPI2, /* Pin 0 (PA0) which is the slave select pin */
	0 /* Max speed setting (18MHz, CPHA=0, CPOL=0, MSb first).*/ /* TODO rrangan - See if we can make it faster as the processor can work at 25 MHz) */
};

/*Minimum speed SPI configuration */
const SPIConfig ls_spicfg = {
	NULL, /* No callback */
	/* Hardware definition starts */
	GPIOA, /* Port on which slave select line is defined */
	GPIOA_CS_SPI2, /* Pin 0 (PA0) which is the slave select pin */
	SPI_CR1_BR_2 | SPI_CR1_BR_1 /* Minimum speed setting (281.250kHz, CPHA=0, CPOL=0, MSb first).*/
};
#endif	/*PLUTO_USE_FATFS */

#if PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER
/*Config for I2C3 */
static const I2CConfig i2cfg3 = {
    OPMODE_I2C,
    100000,
    STD_DUTY_CYCLE,
};
#endif	/*PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER */

#if PLUTO_USE_ACCELEROMETER
/*Config for I2C1 */
static const I2CConfig i2cfg1 = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_16_9,//STD_DUTY_CYCLE,
};
#endif	/*PLUTO_USE_ACCELEROMETER */

/*Initializes SerialDriver1 */
void SD1Init(void) {
	sdStart(&SD1, &usart1);
	palSetPadMode(GPIOB, 6, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOB, 7, PAL_MODE_ALTERNATE(7));
}

/*Initializes I2C Drivers 1 and 3 */
void I2CInitialize(void) {

	// i2cInit() ;

#if PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER
	i2cStart(&I2CD3, &i2cfg3);

	/*To Link PA8 and PC9 to I2C3 function */
	palSetPadMode(GPIOA, 8,  PAL_MODE_ALTERNATE(4)| PAL_STM32_OTYPE_OPENDRAIN);// | PAL_STM32_OTYPE_OPENDRAIN);
	palSetPadMode(GPIOC, 9,  PAL_MODE_ALTERNATE(4)| PAL_STM32_OTYPE_OPENDRAIN);// | PAL_STM32_OTYPE_OPENDRAIN);

	chThdSleepMilliseconds(100);
#endif	/*PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER */

#if PLUTO_USE_ACCELEROMETER
	i2cAcquireBus(&I2CD1);
	i2cStart(&I2CD1, &i2cfg1);

	/*To Link PB8 and PB9 to I2C1 function */
	palSetPadMode(GPIOB, 8,  PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);// | PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_PUDR_PULLUP);// | PAL_STM32_OTYPE_OPENDRAIN);
	palSetPadMode(GPIOB, 9,  PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);//PAL_STM32_OTYPE_OPENDRAINN);// | PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_PUDR_PULLUP);// | PAL_STM32_OTYPE_OPENDRAIN);

	chThdSleepMilliseconds(100);
	i2cReleaseBus(&I2CD1);
	set_mpu_i2c((BaseSequentialStream *)&SD1) ;
#endif	/*PLUTO_USE_ACCELEROMETER */

#if PLUTO_USE_MAGNETOMETER
	/*Set HMC_DRDY pin as input */
	palSetPadMode(HMC_DRDY_PORT, HMC_DRDY_PIN, PAL_MODE_INPUT);
#endif	/*PLUTO_USE_MAGNETOMETER*/

}

/*Initializes the SPI2 Driver */
#if PLUTO_USE_FATFS
void SPI2Init(void) {
	/*
	 * Initializes the SPI driver 2. The SPI2 signals are routed as follow:
	 * PB12 - NSS.
	 * PB13 - SCK.
	 * PB14 - MISO.
	 * PB15 - MOSI.
	 */
	spiStart(&SPID2, &hs_spicfg); /* TODO - rrangan do we have todo here or is it taken care of by  mmcObjectInit */

	palSetPad(GPIOA, GPIOA_CS_SPI2);
	palSetPadMode(GPIOA,GPIOA_CS_SPI2, PAL_MODE_OUTPUT_PUSHPULL); /* Setting Chip Select to PUSHPULL */

	palSetPadMode(GPIOB, 10, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);           /* SCK.     */

	palSetPadMode(GPIOC, 2, PAL_MODE_ALTERNATE(5));              /* MISO.    */
	palSetPadMode(GPIOC, 3, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);           /* MOSI.    */
}
#endif	/*PLUTO_USE_FATFS */
