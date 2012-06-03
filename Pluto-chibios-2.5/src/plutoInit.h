/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains all the functions required to start
 *different drivers like I2C, SPI and USART.
 */

#include "plutoconf.h"

#ifndef PLUTOINIT_H_
#define PLUTOINIT_H_

/*Only done as MMC needs this */
#if PLUTO_USE_FATFS
extern const SPIConfig ls_spicfg ;

extern const SPIConfig hs_spicfg ;

/*Initializes SPI2 Driver */
void SPI2Init(void);
#endif

/*Initializes I2C Drivers 1 and 3 */
#if PLUTO_USE_MAGNETOMETER | PLUTO_USE_BAROMETER | PLUTO_USE_ACCELEROMETER
void I2CInitialize(void);
#endif

/*Initializes SerialDriver1 for Output and Input */
void SD1Init(void);

#endif /* PLUTOINIT_H_ */
