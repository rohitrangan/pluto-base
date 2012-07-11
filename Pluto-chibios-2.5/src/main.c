/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *The file which contains the main function.
 */

#include "pluto.h"

/*
 * Application entry point.
 */

int main(void) {

	/*
	 * System initializations.
	 * - HAL initialization, this also initializes the configured device drivers
	 *   and performs the board-specific initializations.
	 * - Kernel initialization, the main() function becomes a thread and the
	 *   RTOS is active.
	 */
	halInit() ;
	chSysInit() ;

	/*
	 * Activates the SerialDriver for Output and I2C Drivers.
	 */
	OUTPUTInit() ;
	chThdSleepMilliseconds(10) ;
	chprintf((BaseSequentialStream *)&OUTPUT, "\r\nInitializing...\r\n") ;
#if PLUTO_USE_IMU || PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER
	IMUConfig imucfg1 = {
		FS_SEL_250,
		AFS_SEL_2g,
		0x09
	};
	MagConfig magcfg1 = {
		AVERAGE4,
		ODR6,
		MODE_NORMAL,
		RANGE_880mGa,
		OP_MODE_SINGLE
	};
	Sensors init = {
		&imucfg1,
		&IMUD1,
		&magcfg1,
		&MD1,
		ULTRA_HIGH_RESOLUTION,
		&BD1
	};
	I2CInitialize() ;
	chThdSleepMilliseconds(10) ;
	SensorInitialize(&init) ;
	chThdSleepMilliseconds(10) ;
#endif	/*PLUTO_USE_IMU || PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER */

#if HAL_USE_ICU
	icuInit() ;
	chThdSleepMilliseconds(10) ;
#endif	/*HAL_USE_ICU */

#if HAL_USE_PWM
	initPWM() ;
	chThdSleepMilliseconds(10) ;
#endif	/*HAL_USE_PWM */

#if PLUTO_USE_FATFS
	startMMC() ;
	chThdSleepMilliseconds(10) ;
#endif	/*PLUTO_USE_FATFS */
	/*
	 * Shell manager initialization.
	 */
#if PLUTO_USE_SHELL
	Thread *shelltp = NULL ;
	shellInit() ;
#endif	/*PLUTO_USE_SHELL */

#if PLUTO_USE_DCM
	startDCMThread((BaseSequentialStream *)&OUTPUT) ;
#endif	/*PLUTO_USE_DCM */
	while (TRUE) {
#if PLUTO_USE_SHELL
		if (!shelltp)
			shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO) ;
		else if (chThdTerminated(shelltp)) {
			chThdRelease(shelltp) ; /* Recovers memory of the previous shell.   */
			shelltp = NULL ; /* Triggers spawning of a new shell.        */
		}
#endif	/*PLUTO_USE_SHELL */
	}

	return 0 ;
}
