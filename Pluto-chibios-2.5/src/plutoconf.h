/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *PLUTO Features can be enabled and disabled in this file.
 *TODO rohitrangan Incorporate other sensors of PLUTO here.
 */

#ifndef PLUTOCONF_H_
#define PLUTOCONF_H_

/*For using MicroSD Card with a FAT filesystem */
#define PLUTO_USE_FATFS 			FALSE

/*To use the onboard IMU */
#define PLUTO_USE_IMU				TRUE

/*To use the onboard barometer */
#define PLUTO_USE_BAROMETER			TRUE

/*To use the onboard magnetometer */
#define PLUTO_USE_MAGNETOMETER		TRUE

/*To use the onboard pressure transducer */
#define PLUTO_USE_DIFFP				FALSE

/*To use the shell for carrying out commands (DO NOT DISABLE) */
#define PLUTO_USE_SHELL				FALSE

/*To list all the Sensors on board for debugging.(Default should be FALSE) */
#define PLUTO_USE_SCANNER			FALSE

/*To check whether the onboard FPU works */
#define PLUTO_USE_FPUTEST			FALSE

/*To use DCM to calculate the altitude */
#define PLUTO_USE_DCM				TRUE

/*Serial Driver for Output. */
#define OUTPUT						SD1

#endif /* PLUTOCONF_H_ */
