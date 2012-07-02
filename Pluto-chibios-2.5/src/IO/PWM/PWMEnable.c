/*Created By - Rohit Rangan
 *Created On - 02-07-2012
 *
 *This file contains all the functions to start
 *the PWM driver.
 */

#include "ch.h"
#include "hal.h"
#include "PWMInit.h"
#include "PWMEnable.h"

#if HAL_USE_PWM
static WORKING_AREA(PWM_WA, 128) ;

/*This thread starts PWM on Servo 4 and varies the
 *pulse width from 1000-2000 ms in increments of 100.
 */
static msg_t PWMThread(void *arg) {
	(void)arg ;
	chRegSetThreadName("PWMTHREAD") ;

	uint32_t width = 1000 ;
	while(TRUE) {
		pwmEnableChannelI(&PWMD2, SERVO4, PWM_FRACTION_TO_WIDTH(&PWMD2, PWM_PERIOD, width)) ;
		chThdSleepMilliseconds(100) ;
		pwmDisableChannelI(&PWMD2, SERVO4) ;

		if(width > 2000)
			width = 1000 ;
		else
			width += 100 ;
	}

	return 0 ;
}

void startPWMThread(void) {
	chThdCreateStatic(PWM_WA, sizeof(PWM_WA), NORMALPRIO, PWMThread, NULL) ;
}
#endif	/*HAL_USE_PWM */
