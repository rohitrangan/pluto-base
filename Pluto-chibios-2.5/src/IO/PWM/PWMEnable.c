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

/*This function starts the Servo which is specified
 *by the parameter servo. The parameter wid stores the
 *width of the PWM pulse. Width is between 1000-2000 ms.
 */
void startServo(int servo, uint32_t wid) {
	switch(servo) {
		case 1  : pwmEnableChannelI(&PWMD3, SERVO1, PWM_FRACTION_TO_WIDTH(&PWMD3, PWM_PERIOD, wid)) ;
				  break ;
		case 2  : pwmEnableChannelI(&PWMD1, SERVO2, PWM_FRACTION_TO_WIDTH(&PWMD1, PWM_PERIOD, wid)) ;
				  break ;
		case 3  : pwmEnableChannelI(&PWMD8, SERVO3, PWM_FRACTION_TO_WIDTH(&PWMD8, PWM_PERIOD, wid)) ;
				  break ;
		case 4  : pwmEnableChannelI(&PWMD2, SERVO4, PWM_FRACTION_TO_WIDTH(&PWMD2, PWM_PERIOD, wid)) ;
				  break ;
		case 5  : pwmEnableChannelI(&PWMD3, SERVO5, PWM_FRACTION_TO_WIDTH(&PWMD3, PWM_PERIOD, wid)) ;
				  break ;
		case 6  : pwmEnableChannelI(&PWMD3, SERVO6, PWM_FRACTION_TO_WIDTH(&PWMD3, PWM_PERIOD, wid)) ;
				  break ;
		default : return ;
	}
}

/*This function stops the Servo which is specified
 *by the parameter servo.
 */
void stopServo(int servo) {
	switch(servo) {
		case 1  : pwmDisableChannelI(&PWMD3, SERVO1) ;
				  break ;
		case 2  : pwmDisableChannelI(&PWMD1, SERVO2) ;
				  break ;
		case 3  : pwmDisableChannelI(&PWMD8, SERVO3) ;
				  break ;
		case 4  : pwmDisableChannelI(&PWMD2, SERVO4) ;
				  break ;
		case 5  : pwmDisableChannelI(&PWMD3, SERVO5) ;
				  break ;
		case 6  : pwmDisableChannelI(&PWMD3, SERVO6) ;
				  break ;
		default : return ;
	}
}
#endif	/*HAL_USE_PWM */
