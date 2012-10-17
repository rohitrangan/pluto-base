/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains all the functions required to start
 *different drivers like I2C, SPI and USART.
 */

#include "plutoconf.h"

#ifndef PLUTO_H_
#define PLUTO_H_

#include "ch.h"
#include "ff.h"
#include "hal.h"
#include "shell.h"
#include "chprintf.h"

#include "dcm.h"
#include "utils.h"
#include "fsInit.h"
#include "RCInput.h"
#include "PWMInit.h"
#include "IMUData.h"
#include "vector3d.h"
#include "Calibrate.h"
#include "PWMEnable.h"
#include "plutoconf.h"
#include "dcm_update.h"
#include "shellCommands.h"
#include "BarometerData.h"
#include "MagnetometerData.h"

typedef struct {
#if PLUTO_USE_IMU
	IMUConfig 	imucfg   ;
	IMUData 	*imudata ;
#endif	/*PLUTO_USE_IMU */

#if PLUTO_USE_MAGNETOMETER
	MagConfig 	magcfg   ;
	MagData 	*magdata ;
#endif	/*PLUTO_USE_MAGNETOMETER*/

#if PLUTO_USE_BAROMETER
	uint8_t 	barocfg  ;
	BaroData 	*barodata ;
#endif	/*PLUTO_USE_BAROMETER */
}Sensors;

/*Only done as MMC needs this */
#if PLUTO_USE_FATFS
extern const SPIConfig ls_spicfg ;

extern const SPIConfig hs_spicfg ;

/*Initializes SPI2 Driver */
void SPI2Init(void) ;
#endif	/*PLUTO_USE_FATFS */

#if PLUTO_USE_MAGNETOMETER || PLUTO_USE_BAROMETER || PLUTO_USE_IMU
/*Initializes I2C Drivers 1 and 3 */
void I2CInitialize(void) ;

void SensorInitialize(Sensors *sensor) ;
#endif	/*PLUTO_USE_MAGNETOMETER | PLUTO_USE_BAROMETER | PLUTO_USE_IMU */

#if PLUTO_USE_SCANNER
void i2cScanner(I2CDriver *FindI2C, const char *driverName) ;
#endif	/*PLUTO_USE_SCANNER */

/*Initializes SerialDriver1 for Output and Input */
void OUTPUTInit(void) ;

#endif /* PLUTO_H_ */
