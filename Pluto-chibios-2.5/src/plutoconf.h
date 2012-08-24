/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *PLUTO Features can be enabled and disabled in this file.
 */

#ifndef PLUTOCONF_H_
#define PLUTOCONF_H_

/*For using MicroSD Card with a FAT filesystem */
#define PLUTO_USE_FATFS 			TRUE

/*To use the onboard IMU */
#define PLUTO_USE_IMU				TRUE

/*To use the onboard barometer */
#define PLUTO_USE_BAROMETER			TRUE

/*To use the onboard magnetometer */
#define PLUTO_USE_MAGNETOMETER      TRUE

/*To use the onboard pressure transducer */
#define PLUTO_USE_DIFFP				FALSE

/*To use the shell for carrying out commands (DO NOT DISABLE) */
#define PLUTO_USE_SHELL				TRUE

/*To list all the Sensors on board for debugging.(Default should be FALSE) */
#define PLUTO_USE_SCANNER			FALSE

/*To check whether the onboard FPU works */
#define PLUTO_USE_FPUTEST			FALSE

/*To calibrate the IMU */
#define PLUTO_CALIBRATE_IMU			FALSE

/*To use DCM to calculate the altitude */
#define PLUTO_USE_DCM				TRUE

/*To use the RC Controller for input */
#define PLUTO_USE_RC				TRUE

/*To use the ZigBee protocol for serial communication */
#define PLUTO_USE_ZIGBEE			TRUE

#endif	/* PLUTOCONF_H_ */
