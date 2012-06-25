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
  chprintf((BaseSequentialStream *)&SD1, "\r\nInitializing...\r\n") ;
#if PLUTO_USE_IMU || PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER
  I2CInitialize() ;
  //SensorInitialize() ;
#endif	/*PLUTO_USE_IMU || PLUTO_USE_BAROMETER || PLUTO_USE_MAGNETOMETER */
  /*
   * Shell manager initialization.
   */
#if PLUTO_USE_SHELL
  Thread *shelltp = NULL;
  shellInit();
#endif	/*PLUTO_USE_SHELL */
#if PLUTO_USE_FATFS
  startMMC() ;
#endif	/*PLUTO_USE_FATFS */

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and listen for events.
   */
   while(TRUE) {
#if PLUTO_USE_SHELL
	   if(!shelltp)
		   shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
	   else if (chThdTerminated(shelltp)) {
		   chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
		   shelltp = NULL;           /* Triggers spawning of a new shell.        */
	   }
#endif	/*PLUTO_USE_SHELL */
  }
  return 0;
}
