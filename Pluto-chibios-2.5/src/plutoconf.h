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

/*To use the onboard accelerometer */
#define PLUTO_USE_ACCELEROMETER		TRUE

/*To use the onboard barometer */
#define PLUTO_USE_BAROMETER			TRUE

/*To use the onboard magnetometer */
#define PLUTO_USE_MAGNETOMETER		TRUE

/*To use the onboard pressure transducer */
#define PLUTO_USE_TRANSDUCER		FALSE

/*To use the shell for carrying out commands (DO NOT DISABLE) */
#define PLUTO_USE_SHELL				TRUE

/*To list all the Sensors on board for debugging.(Default should be FALSE) */
#define PLUTO_USE_SCANNER			TRUE

#endif /* PLUTOCONF_H_ */
