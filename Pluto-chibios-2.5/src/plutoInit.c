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
#include "chprintf.h"

#include "plutoInit.h"
#include "hmc5883.h"
#include "BMP180.h"
#include "MPU60X0.h"
#include "ms4515.h"
#include "PWMInit.h"

/*Config for USART1 */
static const SerialConfig usartOP = {
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

#if PLUTO_USE_IMU
/*Config for I2C1 */
static const I2CConfig i2cfg1 = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_16_9,//STD_DUTY_CYCLE,
};
#endif	/*PLUTO_USE_IMU */

#if PLUTO_USE_SCANNER
void i2cScanner(I2CDriver *FindI2C, const char *driverName) {

	uint8_t x = 0, txbuf[2], rxbuf[2] ;
	msg_t messages = 0 ;

	for(x = 0 ; x < 128 ; x++){
		txbuf[0] = 0x00 ;
		txbuf[1] = 0x00 ;

		i2cAcquireBus(FindI2C) ;
	    messages = i2cMasterTransmit(FindI2C, x, txbuf, 2, rxbuf, 0) ;
	    i2cReleaseBus(FindI2C) ;
	    if(messages == 0) {
	    	if(x == 0x28)
	    		chprintf((BaseSequentialStream *)&OUTPUT, "MS4515\tDetected on %s\r\n", driverName) ;
	    	if(x == 0x1E)
	    		chprintf((BaseSequentialStream *)&OUTPUT, "HMC\tDetected on %s\r\n", driverName) ;
	    	if(x == 0x68)
	    		chprintf((BaseSequentialStream *)&OUTPUT, "MPU\tDetected on %s\r\n", driverName) ;
	    	if(x == 0x69)
	    		chprintf((BaseSequentialStream *)&OUTPUT, "MPU\tDetected on %s\r\n", driverName) ;
	    	if(x == 0x77)
	    		chprintf((BaseSequentialStream *)&OUTPUT, "BMP\tDetected on %s\r\n", driverName) ;
	    }
		chThdSleepMilliseconds(1) ;
	}
	chThdSleepMilliseconds(100) ;

}
#endif /*PLUTO_USE_SCANNER */

/*Initializes SerialDriver1 */
void OUTPUTInit(void) {
	sdStart(&OUTPUT, &usartOP);
	palSetPadMode(GPIOB, 6, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOB, 7, PAL_MODE_ALTERNATE(7));
}

/*Initializes I2C Drivers 1 and 3 */
void I2CInitialize(void) {

#if PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER
	i2cStart(&I2CD3, &i2cfg3) ;

	/*To Link PA8 and PC9 to I2C3 function */
	palSetPadMode(GPIOA, 8,  PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN) ;
	palSetPadMode(GPIOC, 9,  PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN) ;

	chThdSleepMilliseconds(10) ;
#if PLUTO_USE_SCANNER
	i2cScanner(&I2CD3, "I2C3") ;
#endif	/*PLUTO_USE_SCANNER */

#endif	/*PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER */

#if PLUTO_USE_MAGNETOMETER
	palSetPadMode(HMC_DRDY_PORT, HMC_DRDY_PIN, PAL_MODE_INPUT) ;
#endif	/*PLUTO_USE_MAGNETOMETER */

#if PLUTO_USE_IMU
	i2cStart(&I2CD1, &i2cfg1) ;

	/*To Link PB8 and PB9 to I2C1 function */
	palSetPadMode(GPIOB, 8,  PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN) ;// | PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_PUDR_PULLUP);// | PAL_STM32_OTYPE_OPENDRAIN);
	palSetPadMode(GPIOB, 9,  PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN) ;//PAL_STM32_OTYPE_OPENDRAINN);// | PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_PUDR_PULLUP);// | PAL_STM32_OTYPE_OPENDRAIN);

	chThdSleepMilliseconds(10) ;
#if PLUTO_USE_SCANNER
	i2cScanner(&I2CD1, "I2C1") ;
#endif	/*PLUTO_USE_SCANNER */
	set_mpu_i2c() ;
#endif	/*PLUTO_USE_IMU */

#if PLUTO_USE_MAGNETOMETER
	initialize_HMC(AVERAGE4, ODR6, MODE_NORMAL, RANGE_1300mGa, OP_MODE_SINGLE) ;
	chThdSleepMilliseconds(10) ;
#endif	/*PLUTO_USE_MAGNETOMETER*/

#if PLUTO_USE_BAROMETER
	initialize_bmp180(ULTRA_HIGH_RESOLUTION) ;
	chThdSleepMilliseconds(10) ;
#endif	/*PLUTO_USE_BAROMETER */

}

/*Initializes the SPI2 Driver */
#if PLUTO_USE_FATFS
void SPI2Init(void) {
	/*
	 * Initializes the SPI driver 2. The SPI2 signals are routed as follow:
	 * PA0  - NSS.
	 * PB10 - SCK.
	 * PC2  - MISO.
	 * PC3  - MOSI.
	 */

	palSetPadMode(GPIOA,GPIOA_CS_SPI2, PAL_MODE_OUTPUT_PUSHPULL); /* Setting Chip Select to PUSHPULL */
	palSetPad(GPIOA, GPIOA_CS_SPI2);

	palSetPadMode(GPIOB, 10, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);          /* SCK.     */

	palSetPadMode(GPIOC, 2, PAL_MODE_ALTERNATE(5));                                      /* MISO.    */
	palSetPadMode(GPIOC, 3, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);           /* MOSI.    */
}
#endif	/*PLUTO_USE_FATFS */
