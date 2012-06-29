/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *The file which contains the main function.
 */

#include <string.h>
#include "plutoconf.h"

#include "ch.h"
#include "hal.h"
#include "shell.h"
#include "fsInit.h"
#include "chprintf.h"
#include "plutoInit.h"
#include "shellCommands.h"
#include "IMUData.h"
#include "PWMInit.h"
#include "dcm_update.h"

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
	halInit();
	chSysInit();

	/*
	 * Activates the SerialDriver1 and I2C Drivers.
	 */
	SD1Init() ;
	chThdSleepMilliseconds(10) ;
	chprintf((BaseSequentialStream *)&SD1, "\r\nInitializing...\r\n") ;
#if PLUTO_USE_IMU || PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER
	I2CInitialize() ;
	chThdSleepMilliseconds(10) ;
#endif	/*PLUTO_USE_IMU || PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER */

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

  	/*
  	 * Normal main() thread activity.
  	 */

  	start_thread((BaseSequentialStream *)&SD1);

  	while(TRUE) {
#if PLUTO_USE_SHELL
  		if(!shelltp)
  			shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO) ;
  		else if(chThdTerminated(shelltp)) {
  			chThdRelease(shelltp) ;    /* Recovers memory of the previous shell.   */
  			shelltp = NULL ;		   /* Triggers spawning of a new shell.        */
  		}
#endif	/*PLUTO_USE_SHELL */

#if CORTEX_USE_FPU
  		/*if(!i) {
  			float angle[3] ;
  			eulerAngles(angle) ;
  			chprintf((BaseSequentialStream *)&SD1, "%f ", angle[0]) ;
  			chprintf((BaseSequentialStream *)&SD1, "%f\r\n", angle[1]) ;
  			chThdSleepMilliseconds(100) ;
  		}*/
#endif	/*CORTEX_USE_FPU  */
  	}

  	return 0;
}
